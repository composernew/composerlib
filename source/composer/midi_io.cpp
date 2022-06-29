//
// Created by Carla on 28/06/2022.
//

#include "midi_io.h"

namespace composer {

    void export_to_midi(const std::string &filename, const melody &melody) {
        double duration = 4.;   // min 1  | max 8
        int velocity = 40;      // min 40 | max 100

        smf::MidiFile midi_file;
        int track   = 0;
        int channel = 0;
        int instrument = 1; // Acoustic Grand Piano
        midi_file.addTimbre(track, 0, channel, instrument);

        int ticks_per_quarter_note = melody.get_rhythm();
        int start_tick = 0;
        for (auto &note : melody.get_melody()) {
            int end_tick =
                start_tick + static_cast<int>(duration / 4.0 * ticks_per_quarter_note);
            if (note == 20) {
                start_tick += static_cast<int>(duration / 4.0 * ticks_per_quarter_note);
            }
            else {
                midi_file.addNoteOn (track, start_tick, channel, note, velocity);
                midi_file.addNoteOff(track, end_tick,   channel, note);
                start_tick = end_tick;
            }
        }
        midi_file.write(filename);
    }
}
