#pragma once
#include <Arduino.h>

// Guitar Configuration
#define NUM_STRINGS 6
#define MAX_FRETS 4 // Number of mechanical frets per string (excluding open 0)

// MIDI Config
#define MIDI_NAME "GuitarBot_ESP32"

// Actuator Types
// Define which driver is used for what.
// For this MVP, we assume a mix or unified driver, but let's define the interface first.

// Pin Definitions (Example for direct IO or I2C)
#define I2C_SDA 21
#define I2C_SCL 22

// Servo/Solenoid Parameters
#define SERVO_MIN 150
#define SERVO_MAX 600
#define ACTUATION_TIME_MS 100 // How long to hold the pluck
