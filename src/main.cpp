#include <Arduino.h>
#include "Config.h"
#include "HAL.h"
#include "MidiHandler.h"

void setup() {
    Serial.begin(115200);
    Serial.println("Starting Guitar Robot Firmware...");

    Serial.println("Initializing HAL...");
    HAL::init();

    Serial.println("Initializing MIDI...");
    MidiHandler::init();

    Serial.println("Ready to Rock!");
}

void loop() {
    MidiHandler::loop();
    HAL::update();
    // In the future, actuator timing logic (non-blocking) goes here
    delay(1); // Minimal delay
}
