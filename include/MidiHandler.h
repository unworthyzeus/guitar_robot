#pragma once
#include <Arduino.h>

class MidiHandler {
public:
    static void init();
    static void loop(); // Call in main loop
    
private:
    static void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    static void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity);
};
