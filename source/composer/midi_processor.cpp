/*=============================================================================
   Copyright (c) 2014-2021 Joel de Guzman. All rights reserved.
   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/

#include "midi_processor.h"
void midi_processor::operator()(cycfi::q::midi::note_on msg, std::size_t time) {
    processor::operator()(msg, time);

    float range = 128.0;

    key_ = msg.key();
    auto freq = cycfi::q::midi::note_frequency(key_);
    synthesizer_.phase.set(freq, synthesizer_.sampling_rate());
    synthesizer_.env.trigger(static_cast<float>(msg.velocity()) / range);
}

void midi_processor::operator()(cycfi::q::midi::note_off msg, std::size_t time) {
    processor::operator()(msg, time);

    if (msg.key() == key_)
        synthesizer_.env.release();
}
