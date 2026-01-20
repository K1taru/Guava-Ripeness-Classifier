#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD (I2C address 0x27, 20 columns, 4 rows)
// If 0x27 doesn't work, try 0x3F
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pin assignments
const int mq135Pin = A0;  // MQ135 - Air quality (CO2, NH3, VOCs)
const int mq3Pin   = A1;  // MQ3 - Alcohol and volatile compounds

// LED pins (PWM)
const int redLED   = 5;   // D5 - Unripe indicator
const int greenLED = 6;   // D6 - Ripe indicator

// RIPENESS THRESHOLDS (Calibrated from datasheet measurements)
// Based on 10 guava samples with 5 readings each

// Gas Sensor Thresholds (higher values = more gas detected = riper)
// MQ3 detects ethylene and aromatic esters (PRIMARY indicator)
// Unripe: 312-362 (mean: 339.8, median: 340.5)
// Ripe: 317-442 (mean: 361.92, median: 349)
// Overripe: 377-673 (mean: 516.68, median: 502)
const int MQ3_UNRIPE_MAX = 365;      // ≤365 = unripe (slightly above max 362)
const int MQ3_RIPE_MIN   = 366;      // 366-449 = ripe
const int MQ3_OVERRIPE_MIN = 450;    // ≥450 = overripe (below median 502)

// MQ135 detects VOCs and CO2 (SECONDARY indicator - high overlap in data)
// Unripe: 49-73 (mean: 61.98, median: 64)
// Ripe: 48-71 (mean: 62.32, median: 64)
// Overripe: 52-94 (mean: 73.88, median: 73)
const int MQ135_UNRIPE_MAX = 66;     // ≤66 = unripe
const int MQ135_RIPE_MIN   = 67;     // 67-72 = ripe
const int MQ135_OVERRIPE_MIN = 73;   // ≥73 = overripe (at median)

// Dish Recommendations
const char* unripeDishes[3] = {
  "1.Pickled Guava",
  "2.Guava Chutney",
  "3.Green Guava Salad"
};

const char* ripeDishes[3] = {
  "1.Fresh Guava Juice",
  "2.Guava Smoothie",
  "3.Sliced w/ Salt"
};

const char* overripeDishes[3] = {
  "1.Guava Jam",
  "2.Guava Paste",
  "3.Guava Puree"
};

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Display startup message
  lcd.setCursor(0, 0);
  lcd.print("Guava Ripeness");
  lcd.setCursor(0, 1);
  lcd.print("Classifier v4");
  lcd.setCursor(0, 2);
  lcd.print("Initializing...");

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
  
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read analog pins
  int mq135Raw = analogRead(mq135Pin);
  int mq3Raw = analogRead(mq3Pin);

  float mq135Voltage = mq135Raw * (5.0 / 1023.0);
  float mq3Voltage   = mq3Raw * (5.0 / 1023.0);

  // RIPENESS DETECTION LOGIC
  // Multi-factor approach using gas sensors
  int ripeScore = 0;  // Score: 0=unripe, 1=ripe, 2=overripe
  
  // Average the classification from both sensors
  int mq3Class = 0;  // 0=unripe, 1=ripe, 2=overripe
  int mq135Class = 0;
  
  // MQ3 Classification
  if (mq3Raw >= MQ3_OVERRIPE_MIN) {
    mq3Class = 2;  // Overripe
  } else if (mq3Raw >= MQ3_RIPE_MIN) {
    mq3Class = 1;  // Ripe
  } else {
    mq3Class = 0;  // Unripe
  }
  
  // MQ135 Classification
  if (mq135Raw >= MQ135_OVERRIPE_MIN) {
    mq135Class = 2;  // Overripe
  } else if (mq135Raw >= MQ135_RIPE_MIN) {
    mq135Class = 1;  // Ripe
  } else {
    mq135Class = 0;  // Unripe
  }
  
  // Combined classification (average and round)
  ripeScore = (mq3Class + mq135Class + 1) / 2;  // Simple averaging
  
  // LED Control and Status based on ripeness
  // Using PWM at 60% brightness to protect LEDs without resistors
  String statusText = "";
  const char** currentDishes;
  
  if (ripeScore >= 2) {
    // OVERRIPE: Both LEDs alternating (simulated by both on)
    analogWrite(redLED, 150);
    analogWrite(greenLED, 150);
    statusText = "OVERRIPE";
    currentDishes = overripeDishes;
    Serial.println("STATUS: OVERRIPE!");
  } 
  else if (ripeScore == 1) {
    // RIPE: Green LED ON, Red LED OFF
    analogWrite(greenLED, 150);  // 60% brightness (150/255)
    analogWrite(redLED, 0);
    statusText = "RIPE";
    currentDishes = ripeDishes;
    Serial.println("STATUS: RIPE ✓");
  } 
  else {
    // UNRIPE: Red LED ON, Green LED OFF
    analogWrite(redLED, 150);    // 60% brightness (150/255)
    analogWrite(greenLED, 0);
    statusText = "UNRIPE";
    currentDishes = unripeDishes;
    Serial.println("STATUS: UNRIPE");
  }
  
  // LCD Display (20x4)
  lcd.clear();
  
  // Line 1: Status (Result)
  lcd.setCursor(0, 0);
  lcd.print("Result: ");
  lcd.print(statusText);
  
  // Lines 2-4: Recommended Dishes
  lcd.setCursor(0, 1);
  lcd.print(currentDishes[0]);
  
  lcd.setCursor(0, 2);
  lcd.print(currentDishes[1]);
  
  lcd.setCursor(0, 3);
  lcd.print(currentDishes[2]);


  // Serial Print with Ripeness Classification
  Serial.println("=====================================");
  Serial.print("MQ135 (VOCs) Raw: "); Serial.print(mq135Raw);
  Serial.print(" | Volt: "); Serial.print(mq135Voltage, 3);
  Serial.print(" | Class: "); Serial.println(mq135Class);

  Serial.print("MQ3 (Ethylene/Esters) Raw: "); Serial.print(mq3Raw);
  Serial.print(" | Volt: "); Serial.print(mq3Voltage, 3);
  Serial.print(" | Class: "); Serial.println(mq3Class);
  
  Serial.print("Final Classification: "); Serial.println(ripeScore);
  Serial.println("\nRecommended Dishes:");
  for (int i = 0; i < 3; i++) {
    Serial.print("  "); Serial.println(currentDishes[i]);
  }
  Serial.println("=====================================\n");

  delay(5000);  // 5 seconds between readings
}
