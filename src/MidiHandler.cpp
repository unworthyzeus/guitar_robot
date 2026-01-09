#include "MidiHandler.h"
#include "HAL.h"
#include "Config.h"
#include <BLEMidi.h>

// Standard Tuning reference (MIDI Note numbers)
// E2=40, A2=45, D3=50, G3=55, B3=59, E4=64
const uint8_t STRING_BASE_NOTES[6] = {40, 45, 50, 55, 59, 64};

void MidiHandler::init() {
    BLEMidiServer.begin(MIDI_NAME);
    BLEMidiServer.setOnPlayStatus([](bool playing){
        Serial.printf("MIDI Status: %s\n", playing ? "Playing" : "Stopped");
    });
    
    BLEMidiServer.setNoteOnCallback(onNoteOn);
    BLEMidiServer.setNoteOffCallback(onNoteOff);
}

void MidiHandler::loop() {
    // BLEMidi handles itself mostly in background/callbacks, 
    // but we can put keepalive or debug logic here
}

// Logic to find best string for a note
// Simple algorithm: Find the string where the note is reachable (fret 0-4)
// Prefer open strings.
void mapNoteToString(uint8_t note, int8_t* outString, int8_t* outFret) {
    *outString = -1;
    *outFret = -1;
    
    // 1. Octave Folding Logic
    // Limit: Lowest Note = 40 (E2), Highest Note = 68 (E4 + 4 frets)
    // We try to shift the note into the playable range [40, 68]
    int tempNote = note;
    
    // If too low, shift UP
    while (tempNote < 40) {
        tempNote += 12;
    }
    // If too high, shift DOWN
    while (tempNote > 68) {
        tempNote -= 12;
    }

    // Double check: if after shifting it's still out (e.g. edge cases or if range < 12), we might skip.
    // But our range is 28 semitones, so any note should fit.
    
    // 2. Map to String
    for (int s = 0; s < NUM_STRINGS; s++) {
        int diff = tempNote - STRING_BASE_NOTES[s];
        if (diff >= 0 && diff <= MAX_FRETS) {
            *outString = s;
            *outFret = diff;
            return; // Found a valid position
        }
    }
}

void MidiHandler::onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
    if (velocity == 0) {
        // Some controllers send NoteOn vel=0 for NoteOff
        onNoteOff(channel, note, 0);
        return;
    }
    
    int8_t s, f;
    mapNoteToString(note, &s, &f);
    
    if (s != -1) {
        Serial.printf("NoteOn: %d -> String %d, Fret %d\n", note, s, f);
        GuitarString* str = HAL::getString(s);
        if (str) {
            str->setFret(f);
            str->pluck(); 
            // NOTE: In a real system, you might want to separate "Prepare Fret" and "Pluck" 
            // by a few milliseconds to avoid buzzing. 
            // For now, the setFret function is practically instant but mechanical delay exists.
        }
    } else {
        Serial.printf("NoteOn: %d -> Out of Range\n", note);
    }
}

void MidiHandler::onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity) {
    // Optional: Release fret when note ends?
    // For guitar, you usually let it ring or mute it. 
    // Lifting the fret finger usually mutes it effectively (or buzzes).
    // Let's implement lift for now.
    
    int8_t s, f;
    mapNoteToString(note, &s, &f);
    
    if (s != -1) {
         GuitarString* str = HAL::getString(s);
         if (str) {
             str->releaseFret();
         }
    }
}
