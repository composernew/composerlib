//
// Created by Carla on 04/05/2021.
//

#ifndef COMPOSER_MIDI_PROCESSOR_H
#define COMPOSER_MIDI_PROCESSOR_H

#include <iostream>
#include <vector>
#include <q/support/frequency.hpp>

class SMFReader {

  public:

    SMFReader() = default;

    void read(const std::string &filename);

    std::vector<std::tuple<int,cycfi::q::frequency,cycfi::q::duration>> notes_;
};

#endif // COMPOSER_MIDI_PROCESSOR_H
