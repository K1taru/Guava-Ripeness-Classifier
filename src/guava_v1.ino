// Pin assignments
const int phPin    = A0;
const int mq135Pin = A1;
const int mq3Pin   = A2;

// LED pins (PWM)
const int redLED   = 5;  // D5
const int greenLED = 6;  // D6

// Threshold example (YOU CAN EDIT THIS)
float ripeThreshold = 7.0;  // if pH >= 7 → ripe/high → green LED

void setup() {
  Serial.begin(9600);

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  Serial.println("Starting Sensors + LED System...");
}

void loop() {

  // ------------------------------
  // Read analog pins
  // ------------------------------
  int phRaw = analogRead(phPin);
  int mq135Raw = analogRead(mq135Pin);
  int mq3Raw = analogRead(mq3Pin);

  float phVoltage   = phRaw * (5.0 / 1023.0);
  float mq135Voltage = mq135Raw * (5.0 / 1023.0);
  float mq3Voltage   = mq3Raw * (5.0 / 1023.0);

  // Basic pH estimate (adjust during calibration)
  float pHValue = 7 + ((2.5 - phVoltage) / 0.18);

  // ------------------------------
  // LED Logic (change to your preferred sensor)
  // ------------------------------

  if (pHValue >= ripeThreshold) {
    // Ripe or HIGH value → GREEN LED ON, RED OFF
    analogWrite(greenLED, 255);
    analogWrite(redLED, 0);
  } 
  else {
    // Low or unripe → RED LED ON, GREEN OFF
    analogWrite(redLED, 255);
    analogWrite(greenLED, 0);
  }

  // ------------------------------
  // Serial Print
  // ------------------------------
  Serial.println("=====================================");
  Serial.print("pH: "); Serial.print(pHValue, 2);
  Serial.print(" | Raw: "); Serial.print(phRaw);
  Serial.print(" | Volt: "); Serial.println(phVoltage, 3);

  Serial.print("MQ135 Raw: "); Serial.print(mq135Raw);
  Serial.print(" | Volt: "); Serial.println(mq135Voltage, 3);

  Serial.print("MQ3 Raw: "); Serial.print(mq3Raw);
  Serial.print(" | Volt: "); Serial.println(mq3Voltage, 3);

  Serial.println("=====================================\n");

  delay(700);
}
