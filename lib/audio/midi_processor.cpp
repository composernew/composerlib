/*=============================================================================
   Copyright (c) 2014-2020 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/

#include "midi_processor.h"

void midi_processor::operator()(midi::note_on msg, std::size_t time) {

    _key = msg.key();
    auto freq = midi::note_frequency(_key);
    _synth.phase.set(freq, _synth.sampling_rate());
    _synth.env.trigger(float(msg.velocity()) / 128);
}

void midi_processor::operator()(midi::note_off msg, std::size_t time) {

    if (msg.key() == _key)
        _synth.env.release();
}