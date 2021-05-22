//
// Created by Carla on 04/05/2021.
//

#ifndef COMPOSER_SMF_IO_H
#define COMPOSER_SMF_IO_H

#include <iostream>
#include <vector>
#include <q/support/frequency.hpp>
#include <q/support/midi.hpp>
#include <MidiFile.h>

// The smf_reader class reads a MIDI file and returns a smf::MidiEventList prepared to be used to play MIDI notes.

class smf_io {

  public:

    static smf::MidiFile read(const std::string &filename);
    static void write(smf::MidiFile midiFile, std::string const &filename);
};

#endif // COMPOSER_SMF_IO_H
