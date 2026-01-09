# Firmware Robot Guitarrista

Firmware per a ESP32 per controlar un robot que toca la guitarra via MIDI per Bluetooth (BLE).

## Arquitectura
- **Protocol**: MIDI sobre Bluetooth LE (BLE-MIDI).
- **Hardware**: ESP32 + 2x PCA9685 (I2C) per controlar servos/solenoides.

## Configuració
Consulta el fitxer `walkthrough.md` per a instruccions detallades de cablejat i ús.

## Pinout (Connexions)
- **I2C SDA**: GPIO 21
- **I2C SCL**: GPIO 22

## Dependències
- Adafruit PWM Servo Driver
- BLE-MIDI
