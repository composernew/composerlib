//
// Created by Carla on 04/05/2021.
//

#ifndef COMPOSER_SMF_READER_H
#define COMPOSER_SMF_READER_H

#include <iostream>
#include <vector>
#include <q/support/frequency.hpp>
#include <q/support/midi.hpp>
#include <MidiEventList.h>

// The smf_reader class reads a MIDI file and returns a smf::MidiEventList prepared to be used to play MIDI notes.

class smf_reader {

  public:

    static smf::MidiEventList read(const std::string &filename);
};

#endif // COMPOSER_SMF_READER_H
