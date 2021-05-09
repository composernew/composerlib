//
// Created by Carla on 04/05/2021.
//

#include "smf_reader.h"
#include <MidiFile.h>

smf::MidiEventList smf_reader::read(const std::string &filename) {

    smf::MidiFile midiFile;

    midiFile.read(filename);

    midiFile.doTimeAnalysis();

    return midiFile[0];
}
