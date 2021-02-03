//
// Created by Carla on 03/02/2021.
//

#ifndef COMPOSER_NOTES_H
#define COMPOSER_NOTES_H

#include <q/support/literals.hpp>
#include <q/synth/sin.hpp>
#include <q_io/audio_stream.hpp>

///////////////////////////////////////////////////////////////////////////////
// Synthesize a 440 Hz sine wave.
///////////////////////////////////////////////////////////////////////////////

namespace q = cycfi::q;
using namespace q::literals;

struct sin_synth : q::port_audio_stream
{
    sin_synth(q::frequency freq)
        : port_audio_stream(0, 2)
        , phase(freq, this->sampling_rate())
    {}

    void process(out_channels const& out);

    q::phase_iterator phase;   // The phase iterator
};

#endif // COMPOSER_NOTES_H
