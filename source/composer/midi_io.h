//
// Created by Carla on 28/06/2022.
//

#ifndef COMPOSER_MIDI_IO_H
#define COMPOSER_MIDI_IO_H

#include "composer/melody.h"
#include <MidiFile.h>

namespace composer {

    [[maybe_unused]] void export_to_midi(const std::string &filename, const melody &melody);

    melody_problem import_midi_file(const std::string &filename);
} // namespace composer

#endif // COMPOSER_MIDI_IO_H
