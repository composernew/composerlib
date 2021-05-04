//
// Created by Carla on 04/05/2021.
//

#ifndef COMPOSER_MIDI_PROCESSOR_H
#define COMPOSER_MIDI_PROCESSOR_H

#include <iostream>

class SMFProcessor {

  public:

    SMFProcessor()= default;

    void process(std::string filename);
};

#endif // COMPOSER_MIDI_PROCESSOR_H
