#include <Arduino.h>

#include <BleMouse.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// --- HARDWARE CONFIGURATION ---
const int INDEX_PIN = 14;   // Left Click / Next Slide
const int MIDDLE_PIN = 12;  // Right Click / Scroll

// --- SETTINGS: TIMING ---
const int DOUBLE_TAP_SPEED = 300; // ms (Tap twice within this time)
const int HOLD_TIME_SCROLL = 250; // ms (Hold to start scrolling)
const int HOLD_TOGGLE_MODE = 3000; // ms (Hold BOTH to switch modes)

// --- SETTINGS: MOVEMENT (Dynamic Sensitivity) ---
// Low Speed (Precision) -> High Speed (Fast Travel)
float SENS_MIN = 15.0; 
float SENS_MAX = 45.0; 
float DEADZONE = 0.08;
const float SCROLL_TILT_TRIG = 3.0; 
const int SCROLL_DELAY = 60;        

// --- OBJECTS ---
Adafruit_MPU6050 mpu;
// Initial battery set to 100% (Static) since we removed the logic
BleMouse bleMouse("BioSync Mouse", "ESP32", 100); 

// --- STATE VARIABLES ---
bool isMouseActive = false; 
bool isPresentationMode = false; 

// Timers
unsigned long lastScrollTime = 0; 

// Button State Logic
unsigned long indexTouchStart = 0;
bool indexConfirmed = false;

// Middle Finger Logic (Scroll + Double Tap)
unsigned long middleTouchStart = 0;
int middleTapCount = 0;
unsigned long lastMiddleTapTime = 0;
bool isScrolling = false; 

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  pinMode(INDEX_PIN, INPUT_PULLUP);
  pinMode(MIDDLE_PIN, INPUT_PULLUP);

  bleMouse.begin();
  Serial.println(">> BioSync Started.");

  if (!mpu.begin()) {
    Serial.println("MPU6050 Not Found!");
    while (1) delay(10);
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
}

void loop() {
  
  if (bleMouse.isConnected()) {
    
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    unsigned long now = millis();

    bool indexTouching = (digitalRead(INDEX_PIN) == LOW);
    bool middleTouching = (digitalRead(MIDDLE_PIN) == LOW);

    // ==========================================
    // 1. PRESENTATION MODE TOGGLE
    // ==========================================
    // Hold BOTH fingers for 3 seconds to switch modes
    static unsigned long toggleTimer = 0;
    if (indexTouching && middleTouching) {
      if (toggleTimer == 0) toggleTimer = now;
      if (now - toggleTimer > HOLD_TOGGLE_MODE) {
         isPresentationMode = !isPresentationMode; // Switch!
         
         if (isPresentationMode) Serial.println(">> MODE: PRESENTATION (Cursor OFF)");
         else Serial.println(">> MODE: MOUSE (Cursor ON)");
         
         // Wait until release so we don't click immediately
         while(digitalRead(INDEX_PIN) == LOW || digitalRead(MIDDLE_PIN) == LOW) delay(10);
         toggleTimer = 0;
      }
    } else {
      toggleTimer = 0;
    }

    // ==========================================
    // 2. MOUSE LOGIC (Orientation Check)
    // ==========================================
    if (!isPresentationMode) {
       // Check for FLAT -> Disable Mouse
       if (abs(a.acceleration.z) > 8.0) {
         if (isMouseActive) { isMouseActive = false; }
       }
       // Check for HANDSHAKE -> Active Mouse
       else if (abs(a.acceleration.x) > 8.0) {
         if (!isMouseActive) { isMouseActive = true; }
       }
    }

    // ==========================================
    // 3. INPUT HANDLING
    // ==========================================
    if (isMouseActive || isPresentationMode) {
      
      // --- A. INDEX FINGER (Left Click / Next Slide) ---
      if (indexTouching) {
        if (indexTouchStart == 0) indexTouchStart = now;
        // Press immediately for Dragging (Mouse Mode) or Click (Pres Mode)
        if (!indexConfirmed && (now - indexTouchStart > 20)) {
           if (!bleMouse.isPressed(MOUSE_LEFT)) bleMouse.press(MOUSE_LEFT);
           indexConfirmed = true;
        }
      } else {
        if (indexConfirmed) {
           bleMouse.release(MOUSE_LEFT);
           indexConfirmed = false;
        }
        indexTouchStart = 0;
      }

      // --- B. MIDDLE FINGER (Scroll vs Double Tap) ---
      if (middleTouching) {
        if (middleTouchStart == 0) middleTouchStart = now;

        // If held long -> SCROLL MODE (Only in Mouse Mode)
        if (!isPresentationMode && (now - middleTouchStart > HOLD_TIME_SCROLL)) {
           isScrolling = true;
        }
      } else {
        // Finger Released
        if (middleTouchStart != 0) {
           // If we were NOT scrolling, count this as a TAP
           if (!isScrolling) {
              // Check time since last tap for DOUBLE TAP
              if (now - lastMiddleTapTime < DOUBLE_TAP_SPEED) {
                  // Second tap detected! -> RIGHT CLICK
                  bleMouse.click(MOUSE_RIGHT);
                  middleTapCount = 0; // Reset
              } else {
                  // First tap
                  middleTapCount = 1;
              }
              lastMiddleTapTime = now;
           }
        }
        middleTouchStart = 0;
        isScrolling = false;
      }

      // --- C. MOVEMENT (Only in Mouse Mode) ---
      if (!isPresentationMode) {
        
        if (isScrolling) {
           // --- SCROLLING ---
           float tilt = a.acceleration.y; 
           if (now - lastScrollTime > SCROLL_DELAY) {
              if (tilt > SCROLL_TILT_TRIG) { bleMouse.move(0, 0, 1); lastScrollTime = now; }
              else if (tilt < -SCROLL_TILT_TRIG) { bleMouse.move(0, 0, -1); lastScrollTime = now; }
           }
        } 
        else {
           // --- DYNAMIC CURSOR MOVEMENT (Sniper Assist) ---
           // Only move if not holding middle finger (prevent scroll jitter)
           if (!middleTouching) {
             float rawX = g.gyro.z; 
             float rawY = g.gyro.x; 
             if (abs(rawX) < DEADZONE) rawX = 0;
             if (abs(rawY) < DEADZONE) rawY = 0;

             // Calculate Speed (Angular Velocity)
             float velocity = sqrt(sq(rawX) + sq(rawY));
             
             // Dynamic Math: Slow movement = Low Sensitivity, Fast = High
             float dynamicSens = SENS_MIN;
             if (velocity > 0.5) {
                // Map speed 0.5rad/s - 3.0rad/s to SENS_MIN - SENS_MAX
                dynamicSens = map((long)(velocity*100), 50, 300, (long)SENS_MIN, (long)SENS_MAX);
                if (dynamicSens > SENS_MAX) dynamicSens = SENS_MAX;
             }

             int moveX = -(int)(rawX * dynamicSens);
             int moveY = -(int)(rawY * (dynamicSens * 1.5)); // Y axis usually needs more speed

             if (moveX != 0 || moveY != 0) bleMouse.move(moveY, moveX);
           }
        }
      }
      
    } // End Active Logic
  } 
  delay(10); 
}