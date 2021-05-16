//
// Created by Carla on 04/05/2021.
//

#include "smf_io.h"
#include <MidiFile.h>

smf::MidiEventList smf_io::read(const std::string &filename) {

    smf::MidiFile midiFile;

    midiFile.read(filename);

    midiFile.doTimeAnalysis();

    if (midiFile.getTrackCount() > 1) {

        // Converts multi-track data into a single time sequence.
        // This makes possible to process the returned event list as a single track list
        midiFile.joinTracks();
    }

    return midiFile[0];
}

void smf_io::write(smf::MidiEventList eventList, std::string const &filename) {

    smf::MidiFile midifile;

    midifile[0] = eventList;

    midifile.write(filename);
}
