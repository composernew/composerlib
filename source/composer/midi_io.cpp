//
// Created by Carla on 28/06/2022.
//

#include "midi_io.h"

namespace composer {

    [[maybe_unused]] void export_to_midi(const std::string &filename, const melody &melody) {

        smf::MidiFile midi_file;
        int track   = 0;
        int channel = 0;
        int instrument = 1; // Acoustic Grand Piano
        midi_file.addTimbre(track, 0, channel, instrument);

        midi_file.setTPQ(static_cast<int>(melody.get_rhythm()));
        int start_tick = 0;
        for (const auto &note : melody.get_melody()) {
            int end_tick = start_tick + std::get<1>(note);

            if (std::get<0>(note) == 20) {
                start_tick += std::get<1>(note);
            }
            else {
                midi_file.addNoteOn (track, start_tick, channel, std::get<0>(note), std::get<2>(note));
                midi_file.addNoteOff(track, end_tick,   channel, std::get<0>(note));
                start_tick = end_tick;
            }
        }
        midi_file.write(filename);
    }

    melody_problem import_midi_file(const std::string &filename) {

        smf::MidiFile midi_file;

        midi_file.read(filename);

        if (midi_file.getTrackCount() > 1) {
            midi_file.joinTracks();
        }

        melody_problem problem(midi_file);

        return problem;
    }
} // namespace composer
