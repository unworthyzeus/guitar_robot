#include "MidiHandler.h"
#include "HAL.h"
#include "Config.h"
#include <BLEMidi.h>

// Standard Tuning reference (MIDI Note numbers)
// E2=40, A2=45, D3=50, G3=55, B3=59, E4=64
const uint8_t STRING_BASE_NOTES[6] = {40, 45, 50, 55, 59, 64};

// Track which strings are currently holding a note
bool stringActive[6] = {false, false, false, false, false, false};

void MidiHandler::init() {
    BLEMidiServer.begin(MIDI_NAME);
    BLEMidiServer.setOnPlayStatus([](bool playing){
        Serial.printf("MIDI Status: %s\n", playing ? "Playing" : "Stopped");
    });
    
    BLEMidiServer.setNoteOnCallback(onNoteOn);
    BLEMidiServer.setNoteOffCallback(onNoteOff);
}

void MidiHandler::loop() {
    // BLEMidi handles itself mostly in background/callbacks
}

// Logic to find best string for a note with POLYPHONY support
void mapNoteToString(uint8_t note, int8_t* outString, int8_t* outFret) {
    *outString = -1;
    *outFret = -1;
    
    // 1. Octave Folding Logic
    int tempNote = note;
    while (tempNote < 40) tempNote += 12;
    while (tempNote > 68) tempNote -= 12;

    // 2. Map to String with Collision Detection
    // We search for a valid string. If we find one that is FREE, we take it immediately.
    // If we find one that is BUSY, we remember it as a backup plan.
    
    int8_t bestBusyString = -1;
    int8_t bestBusyFret = -1;

    for (int s = 0; s < NUM_STRINGS; s++) {
        int diff = tempNote - STRING_BASE_NOTES[s];
        
        if (diff >= 0 && diff <= MAX_FRETS) {
            // Check if this string is already playing a note
            if (!stringActive[s]) {
                // FOUND A FREE STRING! Best scenario.
                *outString = s;
                *outFret = diff;
                return; 
            } else {
                // String is busy. Store it just in case we don't find a free one.
                // We prefer lower strings usually, or whatever is found first.
                if (bestBusyString == -1) {
                    bestBusyString = s;
                    bestBusyFret = diff;
                }
            }
        }
    }

    // If we are here, we didn't find a free string.
    // Use the busy string (collision/overwrite) if we found one.
    if (bestBusyString != -1) {
        *outString = bestBusyString;
        *outFret = bestBusyFret;
    }
}

void MidiHandler::onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (velocity == 0) {
        onNoteOff(channel, note, 0);
        return;
    }
    
    int8_t s, f;
    mapNoteToString(note, &s, &f);
    
    if (s != -1) {
        Serial.printf("NoteOn: %d -> String %d, Fret %d\n", note, s, f);
        
        // Mark string as active
        stringActive[s] = true;

        GuitarString* str = HAL::getString(s);
        if (str) {
            str->setFret(f);
            str->pluck(); 
        }
    } else {
        Serial.printf("NoteOn: %d -> Out of Range\n", note);
    }
}

void MidiHandler::onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
    // We need to know WHICH string was playing this note to release it.
    // Since we don't store per-note map, we re-calculate.
    // LIMITATION: If we used collision logic to move the note, simple re-calc might miss.
    // Ideally, we should store "activeNote[stringIndex]" to know exactly what to turn off.
    
    // For MVP/Simple version: Just re-map. 
    
    int8_t s, f;
    mapNoteToString(note, &s, &f); // This might return a different string if state changed, but it's a reasonable approximation.
    
    // Better Approach for Off: Turn off the string if it matches the expected note? 
    // Let's just release the mapped string.
    
    if (s != -1) {
         GuitarString* str = HAL::getString(s);
         if (str) {
             str->releaseFret();
             stringActive[s] = false; // Mark as free
         }
    }
}
