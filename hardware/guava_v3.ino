#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD (I2C address 0x27, 20 columns, 4 rows)
// If 0x27 doesn't work, try 0x3F
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pin assignments
const int phPin    = A0;  // pH sensor
const int mq135Pin = A1;  // MQ135 - Air quality (CO2, NH3, VOCs)
const int mq3Pin   = A2;  // MQ3 - Alcohol and volatile compounds

// LED pins (PWM)
const int redLED   = 5;   // D5 - Unripe indicator
const int greenLED = 6;   // D6 - Ripe indicator

// RIPENESS THRESHOLDS (Calibrate as needed)

// pH Thresholds (Guavas are acidic fruits)
// Unripe: pH 3.5-4.0 (more acidic)
// Ripe:   pH 4.0-4.5 (less acidic)
const float PH_UNRIPE_MAX = 3.8;    // Below this = unripe
const float PH_RIPE_MIN   = 4.2;    // Above this = ripe
// Between 3.8-4.2 = transitioning

// Gas Sensor Thresholds (higher values = more gas detected = riper)
// MQ3 detects ethylene and aromatic esters (increase when ripe)
const int MQ3_RIPE_THRESHOLD = 400;  // Adjust based on calibration

// MQ135 detects VOCs and CO2 (increase during ripening)
const int MQ135_RIPE_THRESHOLD = 300; // Adjust based on calibration

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
  lcd.print("Classifier v3");
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
  int phRaw = analogRead(phPin);
  int mq135Raw = analogRead(mq135Pin);
  int mq3Raw = analogRead(mq3Pin);

  float phVoltage   = phRaw * (5.0 / 1023.0);
  float mq135Voltage = mq135Raw * (5.0 / 1023.0);
  float mq3Voltage   = mq3Raw * (5.0 / 1023.0);

  // Basic pH estimate (adjust during calibration)
  float pHValue = 7 + ((2.5 - phVoltage) / 0.18);

  // RIPENESS DETECTION LOGIC
  // Multi-factor approach: pH + gas sensors
  int ripeScore = 0;  // Score system: higher = riper
  
  // Factor 1: pH (guavas become less acidic when ripe)
  if (pHValue >= PH_RIPE_MIN) {
    ripeScore += 2;  // Strong indicator of ripeness
  } else if (pHValue <= PH_UNRIPE_MAX) {
    ripeScore -= 2;  // Strong indicator of unripeness
  }
  // else: transitioning state (no score change)
  
  // Factor 2: MQ3 (ethylene and aromatic compounds increase when ripe)
  if (mq3Raw >= MQ3_RIPE_THRESHOLD) {
    ripeScore += 1;
  }
  
  // Factor 3: MQ135 (VOCs and CO2 increase during ripening)
  if (mq135Raw >= MQ135_RIPE_THRESHOLD) {
    ripeScore += 1;
  }
  
  // LED Control based on ripeness score
  // Using PWM at 60% brightness to protect LEDs without resistors
  String statusText = "";
  if (ripeScore >= 2) {
    // RIPE: Green LED ON, Red LED OFF
    analogWrite(greenLED, 150);  // 60% brightness (150/255)
    analogWrite(redLED, 0);
    statusText = "RIPE";
    Serial.println("STATUS: RIPE ✓");
  } 
  else {
    // UNRIPE: Red LED ON, Green LED OFF
    analogWrite(redLED, 150);    // 60% brightness (150/255)
    analogWrite(greenLED, 0);
    statusText = "UNRIPE";
    Serial.println("STATUS: UNRIPE");
  }
  
  // LCD Display (20x4)
  lcd.clear();
  
  // Line 1: Status and Score
  lcd.setCursor(0, 0);
  lcd.print("Status: ");
  lcd.print(statusText);
  lcd.setCursor(14, 0);
  lcd.print("S:");
  lcd.print(ripeScore);
  
  // Line 2: pH Value
  lcd.setCursor(0, 1);
  lcd.print("pH: ");
  lcd.print(pHValue, 2);
  lcd.print(" (");
  lcd.print(phRaw);
  lcd.print(")");
  
  // Line 3: MQ135 (VOCs)
  lcd.setCursor(0, 2);
  lcd.print("MQ135: ");
  lcd.print(mq135Raw);
  lcd.print(" VOCs");
  
  // Line 4: MQ3 (Ethylene)
  lcd.setCursor(0, 3);
  lcd.print("MQ3: ");
  lcd.print(mq3Raw);
  lcd.print(" Ethylene");


  // Serial Print with Ripeness Score
  Serial.println("=====================================");
  Serial.print("pH: "); Serial.print(pHValue, 2);
  Serial.print(" | Raw: "); Serial.print(phRaw);
  Serial.print(" | Volt: "); Serial.println(phVoltage, 3);

  Serial.print("MQ135 (VOCs) Raw: "); Serial.print(mq135Raw);
  Serial.print(" | Volt: "); Serial.println(mq135Voltage, 3);

  Serial.print("MQ3 (Ethylene/Esters) Raw: "); Serial.print(mq3Raw);
  Serial.print(" | Volt: "); Serial.println(mq3Voltage, 3);
  
  Serial.print("Ripeness Score: "); Serial.println(ripeScore);
  Serial.println("=====================================\n");

  delay(2000);  // 2 seconds between readings
}
