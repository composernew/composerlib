/*=============================================================================
   Copyright (c) 2014-2020 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/

#ifndef COMPOSER_MIDI_PROCESSOR_H
#define COMPOSER_MIDI_PROCESSOR_H

#include <q_io/midi_stream.hpp>
#include "synthesizer.h"

namespace midi = q::midi;

struct midi_processor : midi::processor {

    using midi::processor::operator();

    midi_processor(sin_synth& synth)
     :_synth(synth)
    {}

    void operator()(midi::note_on msg, std::size_t time);

    void operator()(midi::note_off msg, std::size_t time);

    std::uint8_t    _key;
    sin_synth&      _synth;
};

#endif // COMPOSER_MIDI_PROCESSOR_H
