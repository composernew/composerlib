//
// Created by Carla on 04/05/2021.
//

#include "smf_io.h"

smf::MidiFile smf_io::read(const std::string &filename) {

    smf::MidiFile midiFile;

    midiFile.read(filename);

    midiFile.doTimeAnalysis();

    if (midiFile.getTrackCount() > 1) {

        // Converts multi-track data into a single time sequence.
        // This makes possible to process the returned event list as a single track list
        midiFile.joinTracks();
    }

    return midiFile;
}

void smf_io::write(smf::MidiFile midiFile, std::string const &filename) {
    midiFile.write(filename);
}
