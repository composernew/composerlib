//
// Created by Carla on 04/05/2021.
//

#ifndef COMPOSER_MIDI_PROCESSOR_H
#define COMPOSER_MIDI_PROCESSOR_H

#include <iostream>
#include <audio/synth.h>

class SMFProcessor {

    synth synth_;

  public:

    explicit SMFProcessor(synth &synth)
        : synth_(synth.timbre_) {}

    void process(const std::string &filename);
};

#endif // COMPOSER_MIDI_PROCESSOR_H
