/*=============================================================================
   Copyright (c) 2014-2021 Joel de Guzman. All rights reserved.
   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/

#ifndef COMPOSER_MIDI_PROCESSOR_H
#define COMPOSER_MIDI_PROCESSOR_H

#include <composer/synth.h>
#include <q/support/midi.hpp>

// The midi_processor class uses cycfi::q::midi::processor to handle MIDI messages and play them with a synthesizer.

namespace composer {

    class midi_processor : cycfi::q::midi::processor {

        synth &synthesizer_;

      public:

        using cycfi::q::midi::processor::operator();

        explicit midi_processor(synth &synthesizer)
            : synthesizer_(synthesizer)
        {}

        void operator()(cycfi::q::midi::note_on msg, std::size_t time);

        void operator()(cycfi::q::midi::note_off msg, std::size_t time);
    };
}

#endif // COMPOSER_MIDI_PROCESSOR_H
