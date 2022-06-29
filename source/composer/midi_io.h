//
// Created by Carla on 28/06/2022.
//

#ifndef COMPOSER_MIDI_IO_H
#define COMPOSER_MIDI_IO_H

#include "composer/melody.h"
#include <MidiFile.h>

namespace composer {

    void export_to_midi(const std::string &filename, const melody &melody);
}

#endif // COMPOSER_MIDI_IO_H
