#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD (I2C address 0x27, 20 columns, 4 rows)
// If 0x27 doesn't work, try 0x3F
LiquidCrystal_I2C lcd(0x27, 20, 4);

// ============================================
// PIN ASSIGNMENTS
// ============================================
// I2C Pins (for LCD 20x4)
// SDA = A4 (Arduino Uno/Nano) - Data line
// SCL = A5 (Arduino Uno/Nano) - Clock line

// Analog Pins
const int mq3Pin = A0;      // MQ3 - Alcohol and volatile compounds (ethylene)

// Digital Pins
const int buttonPin = 2;    // D2 - Button module signal pin (3-pin module: VCC, GND, SIG)
const int redLED    = 5;    // D5 - Unripe indicator (PWM)
const int greenLED  = 6;    // D6 - Ripe indicator (PWM)

// ============================================
// TIMING CONSTANTS
// ============================================
const unsigned long TIMEOUT_MS = 300000;  // 5 minutes timeout (300,000 ms)

// State variables
bool scanComplete = false;
unsigned long lastScanTime = 0;

// ============================================
// RIPENESS THRESHOLDS (Calibrated from datasheet measurements)
// Based on 10 guava samples with 5 readings each
// Using MQ3 sensor only (MQ135 removed due to high data overlap/low accuracy)

// MQ3 Gas Sensor Thresholds (ethylene and aromatic esters)
// Higher values = more gas detected = riper fruit
// Unripe: 312-362 (mean: 339.8, median: 340.5)
// Ripe: 317-442 (mean: 361.92, median: 349)
// Overripe: 377-673 (mean: 516.68, median: 502)
const int MQ3_UNRIPE_MAX = 365;      // ≤365 = unripe (slightly above max 362)
const int MQ3_RIPE_MIN   = 366;      // 366-449 = ripe
const int MQ3_OVERRIPE_MIN = 450;    // ≥450 = overripe (below median 502)

// ============================================
// DISH RECOMMENDATIONS (Expanded)
// ============================================
// Unripe Guava Dishes (8 options)
const char* unripeDishes[] = {
  "Pickled Guava",
  "Guava Chutney",
  "Green Guava Salad",
  "Guava Sinigang",
  "Stir-fried Guava",
  "Guava Kimchi",
  "Sour Guava Candy",
  "Guava Achar"
};
const int unripeDishCount = 8;

// Ripe Guava Dishes (10 options)
const char* ripeDishes[] = {
  "Fresh Guava Juice",
  "Guava Smoothie",
  "Sliced w/ Salt",
  "Guava Fruit Salad",
  "Guava Yogurt Bowl",
  "Guava Ice Cream",
  "Guava Popsicle",
  "Guava Shake",
  "Guava Tart",
  "Fresh Guava Snack"
};
const int ripeDishCount = 10;

// Overripe Guava Dishes (8 options)
const char* overripeDishes[] = {
  "Guava Jam",
  "Guava Paste",
  "Guava Puree",
  "Guava Wine",
  "Guava BBQ Sauce",
  "Guava Leather",
  "Guava Compote",
  "Guava Vinegar"
};
const int overripeDishCount = 8;

// Array to hold randomly selected dishes
char* selectedDishes[3];

void setup() {
  Serial.begin(9600);

  // Initialize random seed using floating analog pin
  randomSeed(analogRead(A1));

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Display startup message
  lcd.setCursor(0, 0);
  lcd.print("Guava Ripeness");
  lcd.setCursor(0, 1);
  lcd.print("Classifier v6");
  lcd.setCursor(0, 2);
  lcd.print("Initializing...");

  // Configure pins
  pinMode(buttonPin, INPUT);  // Button module (has built-in pull-up resistor)
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  
  // Blink both LEDs twice with 1 second interval at startup
  // Using PWM at 50% brightness to protect LEDs without resistors
  for (int i = 0; i < 2; i++) {
    analogWrite(redLED, 128);    // 50% brightness (128/255)
    analogWrite(greenLED, 128);  // 50% brightness (128/255)
    delay(1000);
    analogWrite(redLED, 0);
    analogWrite(greenLED, 0);
    delay(1000);
  }

  Serial.println("Starting Sensors + LED System...");
  Serial.println("Press button to start scanning...");
  
  delay(2000);
  lcd.clear();
  
  // Show ready message
  displayReadyScreen();
}

// ============================================
// HELPER FUNCTIONS
// ============================================

// Display ready screen waiting for scan
void displayReadyScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====================");
  lcd.setCursor(0, 1);
  lcd.print("  Place Guava Near");
  lcd.setCursor(0, 2);
  lcd.print("  Press Button Scan");
  lcd.setCursor(0, 3);
  lcd.print("====================");
  
  // Turn off LEDs when ready
  analogWrite(redLED, 0);
  analogWrite(greenLED, 0);
}

// Randomly select 3 unique dishes from a given array
void selectRandomDishes(const char** dishArray, int dishCount) {
  int indices[3];
  int selected = 0;
  
  while (selected < 3) {
    int randIndex = random(0, dishCount);
    bool unique = true;
    
    // Check if already selected
    for (int i = 0; i < selected; i++) {
      if (indices[i] == randIndex) {
        unique = false;
        break;
      }
    }
    
    if (unique) {
      indices[selected] = randIndex;
      selectedDishes[selected] = (char*)dishArray[randIndex];
      selected++;
    }
  }
}

// Check if button is pressed (with debounce)
bool isButtonPressed() {
  if (digitalRead(buttonPin) == LOW) {
    delay(50);  // Debounce delay
    if (digitalRead(buttonPin) == LOW) {
      // Wait for button release
      while (digitalRead(buttonPin) == LOW) {
        delay(10);
      }
      return true;
    }
  }
  return false;
}

void loop() {
  // ============================================
  // WAIT FOR BUTTON PRESS TO START SCAN
  // ============================================
  if (!scanComplete) {
    // Check for button press to initiate scan
    if (isButtonPressed()) {
      Serial.println("\n>> Button pressed! Starting scan...");
      
      // Show scanning message
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Scanning...");
      lcd.setCursor(0, 1);
      lcd.print("Please wait...");
      delay(1000);
      
      // Perform the scan
      performScan();
      
      // Mark scan as complete
      scanComplete = true;
      lastScanTime = millis();
    }
  }
  
  // ============================================
  // WAIT FOR BUTTON OR TIMEOUT AFTER SCAN
  // ============================================
  if (scanComplete) {
    // Check for button press to scan again
    if (isButtonPressed()) {
      Serial.println("\n>> Button pressed! Ready for new scan...");
      scanComplete = false;
      displayReadyScreen();
      return;
    }
    
    // Check for 5-minute timeout
    if (millis() - lastScanTime >= TIMEOUT_MS) {
      Serial.println("\n>> Timeout reached (5 minutes). Ready for new scan...");
      scanComplete = false;
      displayReadyScreen();
      return;
    }
  }
  
  delay(100);  // Small delay to prevent button bounce issues
}

// ============================================
// MAIN SCAN FUNCTION
// ============================================
void performScan() {
  // Read analog pin
  int mq3Raw = analogRead(mq3Pin);
  float mq3Voltage = mq3Raw * (5.0 / 1023.0);

  // RIPENESS DETECTION LOGIC
  // Using MQ3 sensor (ethylene detection)
  int ripeScore = 0;  // Score: 0=unripe, 1=ripe, 2=overripe
  
  // MQ3 Classification
  if (mq3Raw >= MQ3_OVERRIPE_MIN) {
    ripeScore = 2;  // Overripe
  } else if (mq3Raw >= MQ3_RIPE_MIN) {
    ripeScore = 1;  // Ripe
  } else {
    ripeScore = 0;  // Unripe
  }
  
  // LED Control and Status based on ripeness
  // Using PWM at 60% brightness to protect LEDs without resistors
  String statusText = "";
  const char** currentDishArray;
  int currentDishCount;
  
  if (ripeScore >= 2) {
    // OVERRIPE: Both LEDs alternating (simulated by both on)
    analogWrite(redLED, 150);
    analogWrite(greenLED, 150);
    statusText = "OVERRIPE";
    currentDishArray = overripeDishes;
    currentDishCount = overripeDishCount;
    Serial.println("STATUS: OVERRIPE!");
  } 
  else if (ripeScore == 1) {
    // RIPE: Green LED ON, Red LED OFF
    analogWrite(greenLED, 150);  // 60% brightness (150/255)
    analogWrite(redLED, 0);
    statusText = "RIPE";
    currentDishArray = ripeDishes;
    currentDishCount = ripeDishCount;
    Serial.println("STATUS: RIPE ✓");
  } 
  else {
    // UNRIPE: Red LED ON, Green LED OFF
    analogWrite(redLED, 150);    // 60% brightness (150/255)
    analogWrite(greenLED, 0);
    statusText = "UNRIPE";
    currentDishArray = unripeDishes;
    currentDishCount = unripeDishCount;
    Serial.println("STATUS: UNRIPE");
  }
  
  // Randomly select 3 dishes from the appropriate category
  selectRandomDishes(currentDishArray, currentDishCount);
  
  // LCD Display (20x4)
  lcd.clear();
  
  // Line 1: Status (Result)
  lcd.setCursor(0, 0);
  lcd.print("Result: ");
  lcd.print(statusText);
  
  // Lines 2-4: Recommended Dishes (with numbers)
  lcd.setCursor(0, 1);
  lcd.print("1.");
  lcd.print(selectedDishes[0]);
  
  lcd.setCursor(0, 2);
  lcd.print("2.");
  lcd.print(selectedDishes[1]);
  
  lcd.setCursor(0, 3);
  lcd.print("3.");
  lcd.print(selectedDishes[2]);

  // Serial Print with Ripeness Classification
  Serial.println("=====================================");
  Serial.print("MQ3 (Ethylene/Esters) Raw: "); Serial.print(mq3Raw);
  Serial.print(" | Volt: "); Serial.print(mq3Voltage, 3);
  Serial.print(" | Class: "); Serial.println(ripeScore);
  
  Serial.print("Classification: ");
  if (ripeScore == 2) Serial.println("OVERRIPE");
  else if (ripeScore == 1) Serial.println("RIPE");
  else Serial.println("UNRIPE");
  
  Serial.println("\nRecommended Dishes (Randomly Selected):");
  for (int i = 0; i < 3; i++) {
    Serial.print("  "); Serial.print(i + 1); Serial.print(". ");
    Serial.println(selectedDishes[i]);
  }
  Serial.println("\n>> Press button to scan again (or wait 5 min)");
  Serial.println("=====================================\n");
}
