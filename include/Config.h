#pragma once
#include <Arduino.h>

// Guitar Configuration
#define NUM_STRINGS 6
#define MAX_FRETS 4 // Number of mechanical bars (1 to 4)

// MIDI Config
#define MIDI_NAME "GuitarBot_ESP32"

// Pin Definitions (PCA9685 Channels)
// New "Barre" Architecture:
// Channels 0-5: Pluckers for Strings 1-6
// Channels 6-9: Bars for Frets 1-4

#define PCA_ADDR 0x40 // Only 1 driver needed now
#define I2C_SDA 21
#define I2C_SCL 22

// Servo Parameters
#define SERVO_MIN 150
#define SERVO_MAX 600
#define PLUCK_REST 300   // Center position for plucker
#define PLUCK_HIT  400   // Hit position
#define BAR_UP     200   // Bar lift position
#define BAR_DOWN   400   // Bar press position
