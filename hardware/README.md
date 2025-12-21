# Hardware - Legacy Arduino Code

This folder contains the original Arduino-based guava ripeness classifier using sensors.

## Files

- `guava_v1.ino` - Initial version
- `guava_v2.ino` - Improved version
- `guava_v3.ino` - Latest version

## Hardware Requirements

- Arduino Uno
- pH Sensor
- MQ135 Sensor (VOCs/CO2)
- MQ3 Sensor (ethylene/aromatics)
- I2C 20x4 LCD Display
- LEDs (for status indication)

## Features

- Multi-sensor ripeness detection logic
- Visual status with LEDs (ripe/unripe)
- Live sensor values and ripeness status on LCD

## Note

This is the legacy approach using physical sensors. The main project now uses deep learning with image classification for more accurate results.
