# Guava Ripeness Classifier - Wiring Guide

This guide provides detailed instructions for wiring all components of the Guava Ripeness Classifier v6.

---

## 📋 Components Required

| Component | Quantity | Description |
|-----------|----------|-------------|
| Arduino Uno/Nano | 1 | Microcontroller board |
| LCD 20x4 I2C | 1 | Display module with I2C backpack |
| MQ3 Gas Sensor | 1 | Alcohol/Ethylene detection |
| Red LED | 1 | Unripe indicator |
| Green LED | 1 | Ripe indicator |
| Button Module (3-pin) | 1 | Momentary button with VCC, GND, SIG pins |
| Jumper Wires | ~15 | Male-to-Male and Male-to-Female |
| Breadboard | 1 | For prototyping connections |

---

## 🔌 Pin Assignment Summary

### Arduino Uno/Nano Pinout

| Arduino Pin | Component | Function |
|-------------|-----------|----------|
| **A0** | MQ3 Sensor | Analog output (gas reading) |
| **A1** | (Floating) | Random seed source |
| **A4 (SDA)** | LCD I2C | I2C Data Line |
| **A5 (SCL)** | LCD I2C | I2C Clock Line |
| **D2** | Button Module (SIG) | Scan trigger signal |
| **D5 (PWM)** | Red LED | Unripe indicator |
| **D6 (PWM)** | Green LED | Ripe indicator |
| **5V** | MQ3, LCD | Power supply |
| **GND** | All components | Common ground |

---

## 🔧 Wiring Instructions

### 1. LCD 20x4 I2C Display

The LCD uses I2C communication, requiring only 4 wires.

```
LCD I2C Module    →    Arduino
────────────────────────────────
    GND           →    GND
    VCC           →    5V
    SDA           →    A4
    SCL           →    A5
```

**Wiring Diagram:**
```
                    ┌─────────────────┐
                    │   LCD 20x4 I2C  │
                    │                 │
                    │  GND VCC SDA SCL│
                    └──┬───┬───┬───┬──┘
                       │   │   │   │
                       │   │   │   └──────→ A5 (SCL)
                       │   │   └──────────→ A4 (SDA)
                       │   └──────────────→ 5V
                       └──────────────────→ GND
```

> **Note:** The I2C address is typically `0x27`. If the LCD doesn't work, try `0x3F`.

---

### 2. MQ3 Gas Sensor

The MQ3 sensor detects ethylene and alcohol compounds from ripening fruit.

```
MQ3 Sensor        →    Arduino
────────────────────────────────
    VCC           →    5V
    GND           →    GND
    AOUT (A0)     →    A0
    DOUT          →    (Not used)
```

**Wiring Diagram:**
```
                    ┌─────────────┐
                    │  MQ3 Sensor │
                    │             │
                    │ VCC GND AOUT│
                    └──┬───┬───┬──┘
                       │   │   │
                       │   │   └──────────→ A0
                       │   └──────────────→ GND
                       └──────────────────→ 5V
```

> **Warning:** The MQ3 sensor requires a warm-up time of 24-48 hours for accurate readings when first powered on. For subsequent uses, allow 2-5 minutes warm-up.

---

### 3. LED Indicators

Both LEDs use PWM pins for brightness control. The code uses PWM to limit current, but adding a 220Ω resistor is recommended for LED longevity.

```
Red LED           →    Arduino
────────────────────────────────
    Anode (+)     →    D5 (through 220Ω resistor - optional)
    Cathode (-)   →    GND

Green LED         →    Arduino
────────────────────────────────
    Anode (+)     →    D6 (through 220Ω resistor - optional)
    Cathode (-)   →    GND
```

**Wiring Diagram:**
```
        D5 ───[220Ω]───┬──│>|──┬─── GND
                       │  RED  │
                       │       │
        D6 ───[220Ω]───┴──│>|──┴─── GND
                          GREEN
```

**LED Indicator Meanings:**
| State | Red LED | Green LED | Meaning |
|-------|---------|-----------|---------|
| Ready | OFF | OFF | Waiting for button press |
| Unripe | ON | OFF | Guava is unripe |
| Ripe | OFF | ON | Guava is ripe |
| Overripe | ON | ON | Guava is overripe |

---

### 4. Button Module (3-Pin)

The button module has 3 pins: VCC, GND, and SIG (Signal). The module has a built-in pull-up resistor, so SIG outputs LOW when pressed and HIGH when released.

```
Button Module     →    Arduino
────────────────────────────────
    VCC           →    5V
    GND           →    GND
    SIG (Signal)  →    D2
```

**Wiring Diagram:**
```
                    ┌─────────────┐
                    │Button Module│
                    │             │
                    │ VCC GND SIG │
                    └──┬───┬───┬──┘
                       │   │   │
                       │   │   └──────────→ D2
                       │   └──────────────→ GND
                       └──────────────────→ 5V
```

**Button Operation:**
- Press to start a new scan
- After scan completes, press again to perform another scan
- Auto-reset after 5 minutes of inactivity

---

## 📐 Complete Wiring Diagram

```
                                    ARDUINO UNO/NANO
                            ┌─────────────────────────────┐
                            │                             │
    ┌───────────┐           │    D2 ◄─────────────────────┼──── Button SIG
    │  LCD I2C  │           │                             │
    │           │           │    D5 (PWM) ────────────────┼──── Red LED (+)
    │  SDA ─────┼───────────┼──► A4                       │
    │  SCL ─────┼───────────┼──► A5                       │     Green LED (+)
    │  VCC ─────┼───────┐   │    D6 (PWM) ────────────────┼────────┘
    │  GND ─────┼─────┐ │   │                             │
    └───────────┘     │ │   │                             │
                      │ │   │    A0 ◄─────────────────────┼──── MQ3 AOUT
    ┌───────────┐     │ │   │                             │
    │   MQ3     │     │ │   │                             │
    │           │     │ │   │    5V ──────────────────────┼──┬─ MQ3 VCC
    │  AOUT ────┼─────┼─┼───┼──► A0                       │  ├─ LCD VCC
    │  VCC ─────┼─────┼─┼───┼─────────────────────────────┤  └─ Button VCC
    │  GND ─────┼─────┼─┼───┼─────────────────────────────┤
    └───────────┘     │ │   │                             │
                      │ │   │    GND ─────────────────────┼──┬─ MQ3 GND
    ┌─────────────┐   │ │   │                             │  ├─ LCD GND
    │Button Module│   │ │   │                             │  ├─ Red LED (-)
    │  SIG ───────┼───┼─┼───┼──► D2                       │  ├─ Green LED (-)
    │  VCC ───────┼───┼─┼───┼─────────────────────────────┤  └─ Button GND
    │  GND ───────┼───┴─┼───┼─────────────────────────────┤
    └─────────────┘     │   │                             │
                        │   └─────────────────────────────┘
                        │
                        └─── All connected to 5V rail
```

---

## 🔋 Power Considerations

| Component | Current Draw |
|-----------|--------------|
| Arduino Uno | ~50 mA |
| LCD 20x4 I2C | ~20 mA (with backlight) |
| MQ3 Sensor | ~150 mA (during heating) |
| LEDs (each) | ~10-15 mA (at 60% PWM) |
| **Total** | **~250 mA** |

> **Recommendation:** Use a quality USB power supply or 9V adapter with at least 500mA capacity.

---

## ✅ Pre-Flight Checklist

Before powering on, verify:

- [ ] All GND connections share a common ground
- [ ] LCD SDA → A4, SCL → A5
- [ ] MQ3 AOUT → A0
- [ ] Button module: VCC → 5V, GND → GND, SIG → D2
- [ ] Red LED → D5, Green LED → D6
- [ ] No short circuits between 5V and GND
- [ ] All connections are secure

---

## 🚀 First-Time Setup

1. **Upload the Code**
   - Open `guava_v6.ino` in Arduino IDE
   - Select the correct board (Arduino Uno/Nano)
   - Select the correct COM port
   - Click Upload

2. **Initial Power On**
   - The LCD should display "Guava Ripeness Classifier v6"
   - Both LEDs will blink twice during initialization
   - LCD will show "Place Guava Near, Press Button Scan"

3. **Sensor Warm-up**
   - Allow MQ3 sensor to warm up for 2-5 minutes
   - For first-time use, 24-48 hours is recommended for calibration

4. **Testing**
   - Place a guava near the MQ3 sensor
   - Press the button to scan
   - Result and dish recommendations will display
   - Press button again for another scan

---

## 🔍 Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| LCD blank | Wrong I2C address | Try changing `0x27` to `0x3F` in code |
| LCD garbled | Loose connections | Check SDA/SCL wiring |
| No sensor reading | MQ3 not connected | Verify A0 connection |
| Button not working | Wrong pin or wiring | Check D2, 5V, and GND connections |
| LEDs too dim | PWM value too low | Increase PWM value in code |
| Inaccurate readings | Sensor not warmed up | Wait 5+ minutes after power on |

---

## 📝 Notes

- The I2C address scanner sketch can help identify your LCD's address
- Keep the MQ3 sensor away from direct airflow for stable readings
- Position the guava 1-2 cm from the MQ3 sensor for best results
- The 5-minute timeout returns the system to ready state automatically

---

*Last Updated: January 2026*
*Version: 6.0*
