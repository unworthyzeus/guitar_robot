#include <Arduino.h>
#include "Config.h"
#include "HAL.h"
#include "MidiHandler.h"
#include "WebUI.h"

void setup() {
    Serial.begin(115200);
    Serial.println("Starting Guitar Robot Firmware...");

    Serial.println("Initializing HAL...");
    HAL::init();

    Serial.println("Initializing MIDI...");
    MidiHandler::init();
    
    Serial.println("Initializing WebUI...");
    WebUI::init();

    Serial.println("Ready to Rock!");
}

void loop() {
    MidiHandler::loop();
    HAL::loop();
    WebUI::loop();
    delay(1); // Minimal delay
}
