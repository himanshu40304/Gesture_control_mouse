// #include <Arduino.h>

// // --- Pin Definitions ---
// // Connect your copper tape/wire on Index Finger to GPIO 14
// const int INDEX_PIN = 14;

// // Connect your copper tape/wire on Middle Finger to GPIO 12
// const int MIDDLE_PIN = 12;

// // Thumb wire goes to any GND pin on the ESP32

// void setup() {
//   Serial.begin(115200);

//   // Enable internal pull-up resistors
//   // This removes the need for external resistors on your breadboard
//   pinMode(INDEX_PIN, INPUT_PULLUP);
//   pinMode(MIDDLE_PIN, INPUT_PULLUP);

//   Serial.println("--- Finger Contact Test Started ---");
//   Serial.println("Touch Index or Middle finger to Thumb (GND) to test.");
// }

// // void loop() {
// //   // Read the electrical state
// //   // LOW (0) means the circuit is closed (Finger is touching Thumb)
// //   bool isIndexTouching = (digitalRead(INDEX_PIN) == LOW);
// //   bool isMiddleTouching = (digitalRead(MIDDLE_PIN) == LOW);

// //   // Print Status
// //   if (isIndexTouching) {
// //     Serial.println(">> INDEX FINGER Detected! (Left Click Connection Good)");
// //   }

// //   if (isMiddleTouching) {
// //     Serial.println(">> MIDDLE FINGER Detected! (Scroll Connection Good)");
// //   }

// //   // If nothing is touched
// //   if (!isIndexTouching && !isMiddleTouching) {
// //     // Optional: Print dots so you know the ESP32 is still running
// //     // Serial.print(".");
// //   }

// //   delay(100); // Small delay to make the Serial Monitor readable
// // }

// const int STABLE_TIME = 30; // Wait 30ms to confirm it's real
// unsigned long indexTouchStart = 0;
// unsigned long middleTouchStart = 0;
// bool indexConfirmed = false;
// bool middleConfirmed = false;

// void loop() {
//   bool rawReading = (digitalRead(INDEX_PIN) == LOW); // LOW means touching
//   bool rawReading2 = (digitalRead(MIDDLE_PIN) == LOW); // LOW means touching

//   // 1. If we detect a touch, start the timer
//   if (rawReading && indexTouchStart == 0) {
//     indexTouchStart = millis();
//   }
//   if (rawReading2 && middleTouchStart == 0) {
//     middleTouchStart = millis();
//   }

//   // 2. If the touch disappears properly, reset the timer
//   if (!rawReading) {
//     indexTouchStart = 0;

//     indexConfirmed = false;

//   }

//   if (!rawReading2) {

//     middleTouchStart = 0;

//     middleConfirmed = false;
//   }

//   // 3. ONLY verify the click if it has been held LOW for > 30ms
//   if (rawReading && (millis() - indexTouchStart > STABLE_TIME) && !indexConfirmed) {
//     Serial.println(">> Valid right Click Detected!");
//     indexConfirmed = true; // Prevent repeating print for the same hold

//     // Trigger your Mouse Click here
//     // bleMouse.click(MOUSE_LEFT);
//   }

//   if (rawReading2 && (millis() - middleTouchStart > STABLE_TIME) && !middleConfirmed) {
//     Serial.println(">> Valid left Click Detected!");
//     middleConfirmed = true; // Prevent repeating print for the same hold

// //Trigger your Mouse Click here
//     // bleMouse.click(MOUSE_RIGHT);
//   }

//   delay(10);
// }

// #include <Arduino.h>
//  #include <BleMouse.h>

// // --- Configuration ---
// const int INDEX_PIN = 14;   // Left Click
// const int MIDDLE_PIN = 12;  // Right Click
// const int STABLE_TIME = 20; // 30ms Debounce/Glitch filter

// // --- State Variables ---
// BleMouse bleMouse("BioSync Test Mouse", "MUJ", 100);

// // Timer variables for Index (Left)
// unsigned long indexTouchStart = 0;
// bool indexConfirmed = false;

// // Timer variables for Middle (Right)
// unsigned long middleTouchStart = 0;
// bool middleConfirmed = false;

// void setup() {
//   Serial.begin(115200);

//   // Internal Pull-ups (Touch to GND to activate)
//   pinMode(INDEX_PIN, INPUT_PULLUP);
//   pinMode(MIDDLE_PIN, INPUT_PULLUP);

//   Serial.println("Starting BLE Mouse...");
//   bleMouse.begin();
//   Serial.println(">> GOAL: Pair with 'BioSync Test Mouse' in Bluetooth settings now.");
// }

// void loop() {
//   if (bleMouse.isConnected()) {

//     // ==========================================
//     // 1. LEFT CLICK LOGIC (Index Finger)
//     // ==========================================
//     bool indexTouching = (digitalRead(INDEX_PIN) == LOW);

//     // Start Timer
//     if (indexTouching && indexTouchStart == 0) {
//       indexTouchStart = millis();
//     }

//     // Reset Timer if touch disappears
//     if (!indexTouching) {
//       indexTouchStart = 0;
//       indexConfirmed = false;
//     }

//     // Trigger Action (Only once per valid hold)
//     if (indexTouching && !indexConfirmed && (millis() - indexTouchStart > STABLE_TIME)) {
//       Serial.println("[ACTION] Left Click Sent");
//       bleMouse.click(MOUSE_LEFT);
//       indexConfirmed = true;
//     }

//     // ==========================================
//     // 2. RIGHT CLICK LOGIC (Middle Finger)
//     // ==========================================
//     bool middleTouching = (digitalRead(MIDDLE_PIN) == LOW);

//     // Start Timer
//     if (middleTouching && middleTouchStart == 0) {
//       middleTouchStart = millis();
//     }

//     // Reset Timer
//     if (!middleTouching) {
//       middleTouchStart = 0;
//       middleConfirmed = false;
//     }

//     // Trigger Action
//     if (middleTouching && !middleConfirmed && (millis() - middleTouchStart > STABLE_TIME)) {
//       Serial.println("[ACTION] Right Click Sent");
//       bleMouse.click(MOUSE_RIGHT);
//       middleConfirmed = true;
//     }

//   } else {
//     // Optional: Print a dot every second to show it's waiting for connection
//     static unsigned long lastPrint = 0;
//     if (millis() - lastPrint > 1000) {
//       Serial.println("Waiting for Bluetooth Connection...");
//       lastPrint = millis();
//     }
//   }

//   delay(10); // Small delay for stability
// }

// #include <Arduino.h>
// #include <BleMouse.h>
// #include <Wire.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>

// // --- HARDWARE PINOUT ---
// #define PIN_INDEX   14  // Left Click / Drag
// #define PIN_MIDDLE  12  // Right Click
// #define PIN_RING    27  // Scroll Clutch
// #define PIN_BATTERY 34  // Voltage Divider Input

// // --- TUNING PARAMETERS ---
// #define MOUSE_SENSITIVITY  150   // Higher = Slower Cursor
// #define SCROLL_SENSITIVITY 100   // Higher = Slower Scroll
// #define SHAKE_THRESHOLD    20.0  // m/s^2 (Gravity is ~9.8)
// #define DEBOUNCE_MS        30    // Touch signal stability
// #define DRAG_TIME_MS       300   // Time to hold before "Drag" starts

// // --- OBJECTS ---
// BleMouse bleMouse("BioSync Mouse", "ESP32", 100);
// Adafruit_MPU6050 mpu;

// // --- STATE MACHINE ---
// enum MouseState {
//   STATE_IDLE,
//   STATE_LEFT_CLICK,  // Waiting to confirm hold vs click
//   STATE_DRAG,        // Left button held down
//   STATE_RIGHT_CLICK, // Middle finger logic
//   STATE_SCROLL       // Ring finger clutch
// };

// MouseState currentState = STATE_IDLE;

// // --- VARIABLES ---
// // Timers
// unsigned long indexTouchStart = 0;
// unsigned long lastDebounceTime = 0;
// unsigned long lastBatteryCheck = 0;
// unsigned long lastShakeTime = 0;

// // Logic Flags
// bool indexConfirmed = false;
// bool middleHandled = false;

// // Battery Variables
// // Calibrate this: Measure real battery voltage vs printed voltage
// const float RESISTOR_RATIO = 2.0; // If R1 = R2 = 100k
// const float REF_VOLTAGE = 3.3;

// void setup() {
//   Serial.begin(115200);
//   Wire.begin();

//   // 1. Initialize MPU6050
//   if (!mpu.begin()) {
//     Serial.println("ERR: MPU6050 Not Found!");
//     while (1) delay(10);
//   }
//   mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
//   mpu.setGyroRange(MPU6050_RANGE_500_DEG);
//   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

//   // 2. Initialize Pins (INPUT_PULLUP -> Touch Ground to trigger)
//   pinMode(PIN_INDEX, INPUT_PULLUP);
//   pinMode(PIN_MIDDLE, INPUT_PULLUP);
//   pinMode(PIN_RING, INPUT_PULLUP);
//   pinMode(PIN_BATTERY, INPUT);

//   // 3. Initialize BLE
//   bleMouse.begin();
//   Serial.println(">> BioSync Mouse Started. Waiting for connection...");
// }

// // --- HELPER: Read Battery Voltage ---
// void updateBatteryLevel() {
//   // Read raw ADC (0-4095)
//   int rawValue = analogRead(PIN_BATTERY);

//   // Convert to Voltage
//   float voltage = (rawValue / 4095.0) * REF_VOLTAGE * RESISTOR_RATIO;

//   // Convert to Percentage (Map 3.0V - 4.2V to 0 - 100%)
//   // We use integer mapping for BLE
//   int percentage = map((long)(voltage * 100), 300, 420, 0, 100);
//   percentage = constrain(percentage, 0, 100);

//   // Update BLE Service (Shows in Windows/Phone)
//   bleMouse.setBatteryLevel(percentage);

//   // Debug
//   Serial.printf("Bat: %.2fV (%d%%)\n", voltage, percentage);
// }

// // --- HELPER: Check Shake Gesture ---
// void checkShake(sensors_event_t a) {
//   // Calculate total magnitude of acceleration
//   float totalAccel = sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) + sq(a.acceleration.z));

//   // If shake is hard enough AND cooldown passed (2 seconds)
//   if (totalAccel > SHAKE_THRESHOLD && (millis() - lastShakeTime > 2000)) {
//     Serial.println(">> SHAKE DETECTED! Minimizing Windows...");

//     // Win + D (Toggle Desktop)
//     bleMouse.press(KEY_LEFT_GUI); // Windows Key
//     bleMouse.press('d');
//     delay(50);
//     bleMouse.releaseAll();

//     lastShakeTime = millis();
//   }
// }

// void loop() {
//   if (!bleMouse.isConnected()) return;

//   // 1. READ SENSORS
//   sensors_event_t a, g, temp;
//   mpu.getEvent(&a, &g, &temp);
//   unsigned long now = millis();

//   // Read Touch (LOW = Touched)
//   bool touchIndex  = (digitalRead(PIN_INDEX) == LOW);
//   bool touchMiddle = (digitalRead(PIN_MIDDLE) == LOW);
//   bool touchRing   = (digitalRead(PIN_RING) == LOW);

//   // 2. BACKGROUND TASKS
//   checkShake(a);

//   // Check battery every 60 seconds
//   if (now - lastBatteryCheck > 60000) {
//     updateBatteryLevel();
//     lastBatteryCheck = now;
//   }

//   // 3. STATE MACHINE
//   switch (currentState) {

//     // --- STATE: IDLE ---
//     case STATE_IDLE:
//       // Move Cursor (Gyro Z = X, Gyro Y = Y)
//       bleMouse.move(-(int)g.gyro.z / MOUSE_SENSITIVITY, (int)g.gyro.y / MOUSE_SENSITIVITY);

//       // Transitions
//       if (touchRing) {
//         currentState = STATE_SCROLL;
//       }
//       else if (touchIndex) {
//         currentState = STATE_LEFT_CLICK;
//         indexTouchStart = now;
//         indexConfirmed = false;
//       }
//       else if (touchMiddle && !middleHandled) {
//         // Simple Right Click (Instant)
//         bleMouse.click(MOUSE_RIGHT);
//         middleHandled = true; // Prevent spamming
//         currentState = STATE_RIGHT_CLICK;
//       }
//       break;

//     // --- STATE: SCROLL (Clutch) ---
//     case STATE_SCROLL:
//       // Stop moving cursor, Start Scrolling
//       // Tilt Up/Down (Gyro Y) controls scroll
//       // Divide by SCROLL_SENSITIVITY to control speed
//       bleMouse.move(0, 0, (int)g.gyro.y / SCROLL_SENSITIVITY);

//       if (!touchRing) currentState = STATE_IDLE;
//       break;

//     // --- STATE: LEFT CLICK / DRAG CHECK ---
//     case STATE_LEFT_CLICK:
//       // Keep moving cursor while deciding
//       bleMouse.move(-(int)g.gyro.z / MOUSE_SENSITIVITY, (int)g.gyro.y / MOUSE_SENSITIVITY);

//       // A. If held longer than DRAG_TIME -> Start Dragging
//       if (touchIndex && (now - indexTouchStart > DRAG_TIME_MS) && !indexConfirmed) {
//         bleMouse.press(MOUSE_LEFT);
//         currentState = STATE_DRAG;
//         indexConfirmed = true;
//       }

//       // B. If released quickly -> Normal Click
//       if (!touchIndex) {
//         if (!indexConfirmed) {
//           bleMouse.click(MOUSE_LEFT); // It was just a tap
//         }
//         currentState = STATE_IDLE;
//       }
//       break;

//     // --- STATE: DRAGGING ---
//     case STATE_DRAG:
//       // Mouse button is currently HELD down. Move cursor.
//       bleMouse.move(-(int)g.gyro.z / MOUSE_SENSITIVITY, (int)g.gyro.y / MOUSE_SENSITIVITY);

//       // If released, let go
//       if (!touchIndex) {
//         bleMouse.release(MOUSE_LEFT);
//         currentState = STATE_IDLE;
//       }
//       break;

//     // --- STATE: RIGHT CLICK RESET ---
//     case STATE_RIGHT_CLICK:
//       // Wait for user to let go of middle finger before returning to IDLE
//       if (!touchMiddle) {
//         middleHandled = false;
//         currentState = STATE_IDLE;
//       }
//       // Still allow movement
//       bleMouse.move(-(int)g.gyro.z / MOUSE_SENSITIVITY, (int)g.gyro.y / MOUSE_SENSITIVITY);
//       break;
//   }

//   delay(10); // Loop stability
// }

// #include <Wire.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>

// Adafruit_MPU6050 mpu;

// // --- CONFIGURATION ---
// // 8.0 is a good threshold (Gravity is ~9.8)
// // This allows for a little bit of tilt while still detecting the mode
// const float GRAVITY_THRESHOLD = 8.0;

// bool isMouseActive = false; // true = Mouse Mode, false = Typing Mode

// void setup() {
//   Serial.begin(115200);
//   Wire.begin();

//   if (!mpu.begin()) {
//     Serial.println("MPU6050 Not Found!");
//     while (1) delay(10);
//   }

//   // Standard Setup
//   mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
//   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

//   Serial.println("Orientation Test Started...");
// }

// void loop() {
//   sensors_event_t a, g, temp;
//   mpu.getEvent(&a, &g, &temp);

//   // --- ORIENTATION LOGIC ---

//   // 1. Check for FLAT HAND (Typing Mode)
//   // If Z-axis is seeing most of the gravity (> 8.0 m/s^2)
//   if (abs(a.acceleration.z) > GRAVITY_THRESHOLD) {
//     if (isMouseActive) {
//       Serial.println(">> Mode Switch: TYPING (Mouse OFF)");
//       isMouseActive = false;
//     }
//   }

//   // 2. Check for HANDSHAKE (Mouse Mode)
//   // If Y-axis is seeing most of the gravity (> 8.0 m/s^2)
//   // Note: Depending on how you mounted it, this might be X instead of Y.
//   // Test by rotating your hand and seeing which value hits ~9.8
//   else if (abs(a.acceleration.x) > GRAVITY_THRESHOLD) {
//     if (!isMouseActive) {
//       Serial.println(">> Mode Switch: MOUSE ACTIVE");
//       isMouseActive = true;
//     }
//   }

//   // --- ACTION ---
//   if (isMouseActive) {
//     // RUN YOUR MOUSE CODE HERE
//     // bleMouse.move(...)
//   } else {
//     // DO NOTHING (Prevent accidental cursor movement while typing)
//   }

//   delay(100); // Check 10 times a second
// }

// #include <Arduino.h>
// #include <BleMouse.h>
// #include <Wire.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>

// Adafruit_MPU6050 mpu;
// BleMouse bleMouse("BioSync Mouse", "ESP32", 100);

// // Threshold for "Gravity"
// const float GRAVITY_THRESHOLD = 7.0;

// bool isMouseActive = false;

// void setup() {
//   Serial.begin(115200);
//   Wire.begin();
//   bleMouse.begin();

//   if (!mpu.begin()) {
//     Serial.println("MPU6050 Not Found!");
//     while (1) delay(10);
//   }

//   mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
//   mpu.setGyroRange(MPU6050_RANGE_500_DEG);
//   mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

//   Serial.println(">> BioSync Started.");
// }

// void loop() {
//   sensors_event_t a, g, temp;
//   mpu.getEvent(&a, &g, &temp);

//   // --- LOGIC FIX ---

//   // 1. Check for FLAT (Z-Axis detects Gravity) -> MOUSE OFF
//   if (abs(a.acceleration.z) > GRAVITY_THRESHOLD) {
//     if (isMouseActive) {
//       Serial.println(">> Orientation: FLAT detected -> Mouse turning OFF");
//       isMouseActive = false; // <--- FIX: Set to FALSE when flat
//     }
//   }

//   // 2. Check for HANDSHAKE (X-Axis detects Gravity) -> MOUSE ON
//   else if (abs(a.acceleration.x) > GRAVITY_THRESHOLD) {
//     if (!isMouseActive) {
//       Serial.println(">> Orientation: HANDSHAKE detected -> Mouse turning ON");
//       isMouseActive = true; // <--- FIX: Set to TRUE when vertical
//     }
//   }

//   // --- MOVEMENT ---
//   // Only move if the mode is active
//   if (isMouseActive && bleMouse.isConnected()) {

//     // Note: If Up/Down moves Left/Right, swap these two lines.
//     // If Up goes Down, remove the negative sign (-).
//     int moveX = -(int)(g.gyro.z * 30);
//     int moveY = -(int)(g.gyro.x * 25);

//     bleMouse.move(moveY, moveX);
//   }

//   delay(20);
// }

// #include <Arduino.h>
// #include <BleMouse.h>
// #include <Wire.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>

// Adafruit_MPU6050 mpu;
// BleMouse bleMouse("BioSync Mouse", "ESP32", 100);

// // --- TUNING SECTION ---
// const float GRAVITY_THRESHOLD = 8.0;

// // Increase these numbers to make mouse faster
// // Decrease to make it more precise
// float SENSITIVITY_X = 20.0;  // Left/Right Speed
// float SENSITIVITY_Y = 30.0;  // Up/Down Speed (Often needs to be higher)

// // Deadzone: Ignore tiny movements to stop jitter
// // If cursor drifts when still, increase this to 0.1 or 0.15
// float DEADZONE = 0.08;

// bool isMouseActive = false;

// void setup() {
//   Serial.begin(115200);
//   Wire.begin();
//   bleMouse.begin();

//   if (!mpu.begin()) {
//     Serial.println("MPU6050 Not Found!");
//     while (1) delay(10);
//   }

//   mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
//   mpu.setGyroRange(MPU6050_RANGE_500_DEG);

//   // --- CRITICAL FIX FOR LAG ---
//   // Was BAND_21_HZ (Slow & Smooth) -> Changed to BAND_94_HZ (Fast & Snappy)
//   mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);

//   Serial.println(">> High Performance Mode Started.");
// }

// void loop() {
//   sensors_event_t a, g, temp;
//   mpu.getEvent(&a, &g, &temp);

//   // --- 1. ORIENTATION CHECK ---
//   // (Logic remains the same as your working version)
//   if (abs(a.acceleration.z) > GRAVITY_THRESHOLD) {
//     if (isMouseActive) {
//       isMouseActive = false; // Mouse OFF when Flat
//     }
//   }
//   else if (abs(a.acceleration.x) > GRAVITY_THRESHOLD) {
//     if (!isMouseActive) {
//       isMouseActive = true;  // Mouse ON when Handshake
//     }
//   }

//   // --- 2. MOVEMENT ---
//   if (isMouseActive && bleMouse.isConnected()) {

//     // Read Gyro values (in Radians)
//     float rawX = g.gyro.z; // Left/Right motion
//     float rawY = g.gyro.x; // Up/Down motion (Forward/Backward tilt)

//     // Apply Deadzone (If movement is tiny, ignore it)
//     if (abs(rawX) < DEADZONE) rawX = 0;
//     if (abs(rawY) < DEADZONE) rawY = 0;

//     // Convert to Mouse Integers
//     // Note: I added a (-) to rawY. If Up/Down is backwards, remove that minus sign.

//     // int moveX = -(int)(g.gyro.z * 30);
//     // int moveY = -(int)(g.gyro.x * 25);
//     int moveX = -(int)(rawX * SENSITIVITY_X);
//     int moveY = -(int)(rawY * SENSITIVITY_Y);

//     if (moveX != 0 || moveY != 0) {
//       bleMouse.move(moveY, moveX);
//     }
//   }

//   // --- 3. SPEED BOOST ---
//   // Reduced from 20ms to 8ms for smoother 125Hz feel
//   delay(10);
// }

// #include <Arduino.h>
// #include <BleMouse.h>
// #include <Wire.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>

// // --- OBJECTS ---
// Adafruit_MPU6050 mpu;
// BleMouse bleMouse("BioSync Mouse", "ESP32", 100);

// // ==========================================
// // --- CONFIGURATION: MOVEMENT ---
// // ==========================================
// const float GRAVITY_THRESHOLD = 8.0;

// // Speed Settings
// float SENSITIVITY_X = 20.0;  // Left/Right Speed
// float SENSITIVITY_Y = 30.0;  // Up/Down Speed
// float DEADZONE = 0.09;       // Ignore tiny jitters

// // ==========================================
// // --- CONFIGURATION: CLICKS ---
// // ==========================================
// const int INDEX_PIN = 14;   // Left Click
// const int MIDDLE_PIN = 12;  // Right Click
// const int STABLE_TIME = 20; // 20ms debounce

// // --- VARIABLES ---
// bool isMouseActive = false;

// // Click Timers
// unsigned long indexTouchStart = 0;
// bool indexConfirmed = false;
// unsigned long middleTouchStart = 0;
// bool middleConfirmed = false;

// void setup() {
//   Serial.begin(115200);
//   Wire.begin();

//   pinMode(INDEX_PIN, INPUT_PULLUP);
//   pinMode(MIDDLE_PIN, INPUT_PULLUP);

//   bleMouse.begin();
//   Serial.println(">> BioSync Mouse Started.");

//   if (!mpu.begin()) {
//     Serial.println("MPU6050 Not Found!");
//     while (1) delay(10);
//   }

//   mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
//   mpu.setGyroRange(MPU6050_RANGE_500_DEG);

//   // Fast Bandwidth for low latency
//   mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
// }

// void loop() {
//   if (bleMouse.isConnected()) {

//     // =================================================
//     // PART A: ORIENTATION CHECK
//     // =================================================
//     sensors_event_t a, g, temp;
//     mpu.getEvent(&a, &g, &temp);

//     // 1. Check for FLAT (Typing Mode) -> MOUSE OFF
//     if (abs(a.acceleration.z) > GRAVITY_THRESHOLD) {
//       if (isMouseActive) isMouseActive = false;
//       //Serial.print("Mouse is active");
//     }
//     // 2. Check for HANDSHAKE (Mouse Mode) -> MOUSE ON
//     else if (abs(a.acceleration.x) > GRAVITY_THRESHOLD) {
//       if (!isMouseActive) isMouseActive = true;
//       //Serial.print("Typing is active");
//     }

//     // =================================================
//     // PART B: ACTIVE MOUSE LOGIC
//     // =================================================
//     // Everything inside this 'if' ONLY works when hand is vertical
//     if (isMouseActive) {

//       // --- 1. MOVEMENT ---
//       float rawX = g.gyro.z; // Left/Right
//       float rawY = g.gyro.x; // Up/Down

//       if (abs(rawX) < DEADZONE) rawX = 0;
//       if (abs(rawY) < DEADZONE) rawY = 0;

//       int moveX = -(int)(rawX * SENSITIVITY_X);
//       int moveY = -(int)(rawY * SENSITIVITY_Y);

//       if (moveX != 0 || moveY != 0) {
//         bleMouse.move(moveY, moveX);
//       }

//       // --- 2. CLICKS (Now disabled when typing) ---
//       unsigned long now = millis();

//       // LEFT CLICK
//       bool indexTouching = (digitalRead(INDEX_PIN) == LOW);
//       if (indexTouching && indexTouchStart == 0) indexTouchStart = now;
//       if (!indexTouching) { indexTouchStart = 0; indexConfirmed = false; }

//       if (indexTouching && !indexConfirmed && (now - indexTouchStart > STABLE_TIME)) {
//         bleMouse.click(MOUSE_LEFT);
//         indexConfirmed = true;
//       }

//       // RIGHT CLICK
//       bool middleTouching = (digitalRead(MIDDLE_PIN) == LOW);
//       if (middleTouching && middleTouchStart == 0) middleTouchStart = now;
//       if (!middleTouching) { middleTouchStart = 0; middleConfirmed = false; }

//       if (middleTouching && !middleConfirmed && (now - middleTouchStart > STABLE_TIME)) {
//         bleMouse.click(MOUSE_RIGHT);
//         middleConfirmed = true;
//       }
//     }
//     // End of isMouseActive block
//     // If isMouseActive is false, code does nothing (Safety Mode)

//   }
//   delay(10);
// }

// #include <Arduino.h>
// #include <BleMouse.h>
// #include <Wire.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>

// // --- OBJECTS ---
// Adafruit_MPU6050 mpu;
// BleMouse bleMouse("BioSync Mouse", "ESP32", 100);

// // ==========================================
// // --- CONFIGURATION: MOVEMENT ---
// // ==========================================
// const float GRAVITY_THRESHOLD = 8.0;

// // Speed Settings
// float SENSITIVITY_X = 20.0;
// float SENSITIVITY_Y = 30.0;
// float SCROLL_SPEED = 5.0; // Divider for scroll speed (Lower = Faster)
// float DEADZONE = 0.09;

// // ==========================================
// // --- CONFIGURATION: CLICKS ---
// // ==========================================
// const int INDEX_PIN = 14;
// const int MIDDLE_PIN = 12;
// const int STABLE_TIME = 20;
// const int HOLD_TIME_SCROLL = 250; // ms to trigger scroll mode

// // --- VARIABLES ---
// bool isMouseActive = false;

// // Click Timers
// unsigned long indexTouchStart = 0;
// bool indexConfirmed = false;

// unsigned long middleTouchStart = 0;
// bool middleConfirmed = false;
// bool isScrolling = false; // Flag for Scroll Mode

// void setup()
// {
//   Serial.begin(115200);
//   Wire.begin();

//   pinMode(INDEX_PIN, INPUT_PULLUP);
//   pinMode(MIDDLE_PIN, INPUT_PULLUP);

//   bleMouse.begin();
//   Serial.println(">> BioSync Mouse Started.");

//   if (!mpu.begin())
//   {
//     Serial.println("MPU6050 Not Found!");
//     while (1)
//       delay(10);
//   }

//   mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
//   mpu.setGyroRange(MPU6050_RANGE_500_DEG);
//   mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
// }

// void loop()
// {
//   if (bleMouse.isConnected())
//   {

//     // =================================================
//     // PART A: ORIENTATION CHECK
//     // =================================================
//     sensors_event_t a, g, temp;
//     mpu.getEvent(&a, &g, &temp);

//     // 1. Check for FLAT (Typing Mode) -> MOUSE OFF
//     if (abs(a.acceleration.z) > GRAVITY_THRESHOLD)
//     {
//       if (isMouseActive)
//         isMouseActive = false;
//     }
//     // 2. Check for HANDSHAKE (Mouse Mode) -> MOUSE ON
//     else if (abs(a.acceleration.x) > GRAVITY_THRESHOLD)
//     {
//       if (!isMouseActive)
//         isMouseActive = true;
//     }

//     // =================================================
//     // PART B: ACTIVE MOUSE LOGIC
//     // =================================================
//     if (isMouseActive)
//     {

//       unsigned long now = millis();
//       bool indexTouching = (digitalRead(INDEX_PIN) == LOW);
//       bool middleTouching = (digitalRead(MIDDLE_PIN) == LOW);

//       // --- 1. INDEX FINGER (Left Click / Drag) ---
//       // Standard logic: Press on touch, Release on let go
//       if (indexTouching)
//       {
//         if (indexTouchStart == 0)
//           indexTouchStart = now; // Start timer

//         // If held longer than debounce, register press
//         if (!indexConfirmed && (now - indexTouchStart > STABLE_TIME))
//         {
//           if (!bleMouse.isPressed(MOUSE_LEFT))
//             bleMouse.press(MOUSE_LEFT);
//           indexConfirmed = true;
//         }
//       }
//       else
//       {
//         // Finger released
//         if (indexConfirmed)
//         {
//           bleMouse.release(MOUSE_LEFT);
//           indexConfirmed = false;
//         }
//         indexTouchStart = 0;
//       }

//       // --- 2. MIDDLE FINGER (Right Click OR Scroll) ---
//       if (middleTouching)
//       {
//         if (middleTouchStart == 0)
//           middleTouchStart = now;

//         // CHECK: Have we held it long enough to start SCROLLING?
//         if ((now - middleTouchStart > HOLD_TIME_SCROLL))
//         {
//           isScrolling = true;
//         }
//       }
//       else
//       {
//         // Finger Released
//         if (middleTouchStart != 0)
//         {
//           // If we released QUICKLY (< 200ms) -> It was a CLICK
//           if (!isScrolling && (now - middleTouchStart > STABLE_TIME))
//           {
//             bleMouse.click(MOUSE_RIGHT);
//           }
//         }
//         // Reset everything
//         middleTouchStart = 0;
//         isScrolling = false;
//       }

//       // --- 3. MOVEMENT & SCROLLING PHYSICS ---

//       float rawX = g.gyro.z; // Left/Right
//       float rawY = g.gyro.x; // Up/Down

//       if (abs(rawX) < DEADZONE)
//         rawX = 0;
//       if (abs(rawY) < DEADZONE)
//         rawY = 0;

//       // MODE A: SCROLLING (Middle Finger Held)
//       if (isScrolling)
//       {
//         // Use Gyro Y (Tilt Up/Down) to scroll
//         // Divide by SCROLL_SPEED to control sensitivity
//         int scrollAmount = (int)(rawY * SENSITIVITY_Y / SCROLL_SPEED);

//         // Only scroll if movement is significant
//         if (scrollAmount != 0)
//         {
//           bleMouse.move(0, 0, scrollAmount); // 3rd argument is Scroll Wheel
//         }
//       }
//       // MODE B: CURSOR MOVING (Normal)
//       else
//       {
//         // Freeze cursor if left clicking (Sniper Freeze)
//         if (!indexTouching)
//         {
//           int moveX = -(int)(rawX * SENSITIVITY_X);
//           int moveY = -(int)(rawY * SENSITIVITY_Y);

//           if (moveX != 0 || moveY != 0)
//           {
//             bleMouse.move(moveY, moveX);
//           }
//         }
//       }

//     } // End isMouseActive
//   }
//   delay(10);
// }

// #include <Arduino.h>
// #include <BleMouse.h>
// #include <Wire.h>
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>

// // --- OBJECTS ---
// Adafruit_MPU6050 mpu;
// BleMouse bleMouse("BioSync Mouse", "ESP32", 100);

// // ==========================================
// // --- CONFIGURATION: MOVEMENT ---
// // ==========================================
// const float GRAVITY_THRESHOLD = 8.0;

// // Mouse Speed
// float SENSITIVITY_X = 20.0;
// float SENSITIVITY_Y = 30.0;
// float DEADZONE = 0.09;

// // ==========================================
// // --- CONFIGURATION: SCROLL (NEW) ---
// // ==========================================
// // Tilt Angle to start scrolling (m/s^2).
// // 2.0 is a small tilt, 5.0 is a steep tilt.
// const float SCROLL_TILT_TRIG = 3.0;

// // Scroll Speed (Constant)
// // Time in milliseconds between scroll steps.
// // 100 = Slow, 30 = Fast.
// const int SCROLL_DELAY = 80;

// // ==========================================
// // --- CONFIGURATION: CLICKS ---
// // ==========================================
// const int INDEX_PIN = 14;
// const int MIDDLE_PIN = 12;
// const int STABLE_TIME = 20;
// const int HOLD_TIME_SCROLL = 200;

// // --- VARIABLES ---
// bool isMouseActive = false;

// // Click Timers
// unsigned long indexTouchStart = 0;
// bool indexConfirmed = false;
// unsigned long middleTouchStart = 0;
// bool middleConfirmed = false;
// bool isScrolling = false;
// unsigned long lastScrollTime = 0; // Timer for constant speed

// void setup()
// {
//   Serial.begin(115200);
//   Wire.begin();

//   pinMode(INDEX_PIN, INPUT_PULLUP);
//   pinMode(MIDDLE_PIN, INPUT_PULLUP);

//   bleMouse.begin();
//   Serial.println(">> BioSync Mouse Started.");

//   if (!mpu.begin())
//   {
//     Serial.println("MPU6050 Not Found!");
//     while (1)
//       delay(10);
//   }

//   mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
//   mpu.setGyroRange(MPU6050_RANGE_500_DEG);
//   mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
// }

// void loop()
// {
//   if (bleMouse.isConnected())
//   {

//     // =================================================
//     // PART A: ORIENTATION CHECK
//     // =================================================
//     sensors_event_t a, g, temp;
//     mpu.getEvent(&a, &g, &temp);

//     // 1. Check for FLAT (Typing Mode) -> MOUSE OFF
//     if (abs(a.acceleration.z) > GRAVITY_THRESHOLD)
//     {
//       if (isMouseActive)
//         isMouseActive = false;
//     }
//     // 2. Check for HANDSHAKE (Mouse Mode) -> MOUSE ON
//     else if (abs(a.acceleration.x) > GRAVITY_THRESHOLD)
//     {
//       if (!isMouseActive)
//         isMouseActive = true;
//     }

//     // =================================================
//     // PART B: ACTIVE MOUSE LOGIC
//     // =================================================
//     if (isMouseActive)
//     {

//       unsigned long now = millis();
//       bool indexTouching = (digitalRead(INDEX_PIN) == LOW);
//       bool middleTouching = (digitalRead(MIDDLE_PIN) == LOW);

//       // --- 1. INDEX FINGER (Left Click) ---
//       // This stays "Click on Press" for fast response
//       if (indexTouching)
//       {
//         if (indexTouchStart == 0)
//           indexTouchStart = now;
//         if (!indexConfirmed && (now - indexTouchStart > STABLE_TIME))
//         {
//           if (!bleMouse.isPressed(MOUSE_LEFT))
//             bleMouse.press(MOUSE_LEFT);
//           indexConfirmed = true;
//         }
//       }
//       else
//       {
//         if (indexConfirmed)
//         {
//           bleMouse.release(MOUSE_LEFT);
//           indexConfirmed = false;
//         }
//         indexTouchStart = 0;
//       }

//       // --- 2. MIDDLE FINGER (The Fix) ---
//       if (middleTouching)
//       {
//         // A. Finger just touched -> Start Timer
//         if (middleTouchStart == 0)
//           middleTouchStart = now;

//         // B. Check if held long enough for SCROLLING
//         // DO NOT CLICK HERE! Just change the mode.
//         if ((now - middleTouchStart > HOLD_TIME_SCROLL))
//         {
//           isScrolling = true;
//         }
//       }
//       else
//       {
//         // C. Finger Released
//         if (middleTouchStart != 0)
//         {

//           // Logic: If we were NOT scrolling, and the tap was real -> Right Click
//           if (!isScrolling && (now - middleTouchStart > STABLE_TIME))
//           {
//             bleMouse.click(MOUSE_RIGHT);
//           }
//         }
//         // D. Reset everything
//         middleTouchStart = 0;
//         isScrolling = false;
//       }

//       // --- 3. MOVEMENT & SCROLLING ---
//       if (isScrolling)
//       {
//         // Auto-Scroll Mode (Tilt to Scroll)
//         float tilt = a.acceleration.y; // Use Y (or Z depending on orientation)

//         if (now - lastScrollTime > SCROLL_DELAY)
//         {
//           if (tilt > SCROLL_TILT_TRIG)
//           {
//             bleMouse.move(0, 0, 1); // Scroll UP
//             lastScrollTime = now;
//           }
//           else if (tilt < -SCROLL_TILT_TRIG)
//           {
//             bleMouse.move(0, 0, -1); // Scroll DOWN
//             lastScrollTime = now;
//           }
//         }
//       }
//       else
//       {
//         // Normal Mouse Movement (Sniper Freeze enabled)
//         if (!indexTouching)
//         {
//           float rawX = g.gyro.z;
//           float rawY = g.gyro.x;

//           if (abs(rawX) < DEADZONE)
//             rawX = 0;
//           if (abs(rawY) < DEADZONE)
//             rawY = 0;

//           int moveX = -(int)(rawX * SENSITIVITY_X);
//           int moveY = -(int)(rawY * SENSITIVITY_Y);

//           if (moveX != 0 || moveY != 0)
//           {
//             bleMouse.move(moveY, moveX);
//           }
//         }
//       }

//     } // End isMouseActive
//   }
//   delay(10);
// }

#include <Arduino.h>
#include <BleMouse.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// --- OBJECTS ---
Adafruit_MPU6050 mpu;
BleMouse bleMouse("BioSync Mouse", "ESP32", 100);

// ==========================================
// --- CONFIGURATION: MOVEMENT ---
// ==========================================
const float GRAVITY_THRESHOLD = 8.0; 

// Mouse Speed
float SENSITIVITY_X = 20.0;  
float SENSITIVITY_Y = 30.0;   
float DEADZONE = 0.09;      

// ==========================================
// --- CONFIGURATION: SCROLL ---
// ==========================================
const float SCROLL_TILT_TRIG = 3.0; // Angle to trigger scroll
const int SCROLL_DELAY = 60;        // Scroll speed

// ==========================================
// --- CONFIGURATION: CLICKS ---
// ==========================================
const int INDEX_PIN = 14;   
const int MIDDLE_PIN = 12;  
const int STABLE_TIME = 20; 
const int HOLD_TIME_SCROLL = 200; 

// --- VARIABLES ---
bool isMouseActive = false; 

// Click Timers
unsigned long indexTouchStart = 0;
bool indexConfirmed = false;
unsigned long middleTouchStart = 0;
bool middleConfirmed = false;

// Scroll Variables
bool isScrolling = false; 
unsigned long lastScrollTime = 0; 

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  pinMode(INDEX_PIN, INPUT_PULLUP);
  pinMode(MIDDLE_PIN, INPUT_PULLUP);

  bleMouse.begin();
  Serial.println(">> BioSync Mouse Started.");

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
    
    // =================================================
    // PART A: ORIENTATION CHECK
    // =================================================
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // 1. Check for FLAT (Typing Mode) -> MOUSE OFF
    if (abs(a.acceleration.z) > GRAVITY_THRESHOLD) {
      if (isMouseActive) isMouseActive = false; 
    }
    // 2. Check for HANDSHAKE (Mouse Mode) -> MOUSE ON
    else if (abs(a.acceleration.x) > GRAVITY_THRESHOLD) {
      if (!isMouseActive) isMouseActive = true; 
    }

    // =================================================
    // PART B: ACTIVE MOUSE LOGIC
    // =================================================
    if (isMouseActive) {
      
      unsigned long now = millis();
      bool indexTouching = (digitalRead(INDEX_PIN) == LOW);
      bool middleTouching = (digitalRead(MIDDLE_PIN) == LOW);

      // --- 1. INDEX FINGER (Click & Drag) ---
      if (indexTouching) {
        // A. Just Touched -> Start Timer
        if (indexTouchStart == 0) indexTouchStart = now;
        
        // B. Touched longer than debounce -> PRESS DOWN (Hold)
        if (!indexConfirmed && (now - indexTouchStart > STABLE_TIME)) {
           if (!bleMouse.isPressed(MOUSE_LEFT)) {
               bleMouse.press(MOUSE_LEFT); // Hold button down
           }
           indexConfirmed = true;
        }
      } else {
        // C. Released -> RELEASE BUTTON
        if (indexConfirmed) {
           bleMouse.release(MOUSE_LEFT); // Let go of button
           indexConfirmed = false;
        }
        indexTouchStart = 0;
      }

      // --- 2. MIDDLE FINGER (Right Click OR Scroll) ---
      if (middleTouching) {
        if (middleTouchStart == 0) middleTouchStart = now;

        // Held long enough? -> Enable SCROLL MODE
        if ((now - middleTouchStart > HOLD_TIME_SCROLL)) {
           isScrolling = true;
        }

      } else {
        // Released
        if (middleTouchStart != 0) {
          // If released quickly (Tap) -> RIGHT CLICK
          if (!isScrolling && (now - middleTouchStart > STABLE_TIME)) {
             bleMouse.click(MOUSE_RIGHT);
          }
        }
        middleTouchStart = 0;
        isScrolling = false;
      }

      // --- 3. MOVEMENT & SCROLLING ---
      
      if (isScrolling) {
         // --- SCROLL MODE (Tilt) ---
         float tilt = a.acceleration.y; 

         if (now - lastScrollTime > SCROLL_DELAY) {
            if (tilt > SCROLL_TILT_TRIG) {
               bleMouse.move(0, 0, 1); // Scroll UP
               lastScrollTime = now;
            }
            else if (tilt < -SCROLL_TILT_TRIG) {
               bleMouse.move(0, 0, -1); // Scroll DOWN
               lastScrollTime = now;
            }
         }
      }
      else {
         // --- CURSOR MODE (Move) ---
         // "Sniper Freeze" removed for Index finger so you can Drag.
         // We only freeze if Middle finger is touched (to prevent cursor jump when right clicking)
         
         if (!middleTouching) { 
             float rawX = g.gyro.z; 
             float rawY = g.gyro.x; 

             if (abs(rawX) < DEADZONE) rawX = 0;
             if (abs(rawY) < DEADZONE) rawY = 0;

             int moveX = -(int)(rawX * SENSITIVITY_X);
             int moveY = -(int)(rawY * SENSITIVITY_Y);

             if (moveX != 0 || moveY != 0) {
               bleMouse.move(moveY, moveX);
             }
         }
      }

    } // End isMouseActive
  } 
  delay(10); 
}