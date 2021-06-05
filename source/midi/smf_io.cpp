//
// Created by Carla on 04/05/2021.
//

#include "smf_io.h"

bool smf_io::read(const std::string &filename) {

    if (midiFile.read(filename)){

        midiFile.doTimeAnalysis();

        if (midiFile.getTrackCount() > 1) {

            // Converts multi-track data into a single time sequence.
            // This makes possible to process the returned event list as a single track list
            midiFile.joinTracks();
        }

        return true;
    }

    return false;
}

bool smf_io::write(smf::MidiFile midiFile, std::string const &filename) {

    return midiFile.write(filename);
}

smf::MidiFile smf_io::get_midifile() {

    return midiFile;
}
