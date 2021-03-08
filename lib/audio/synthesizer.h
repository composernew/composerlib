/*=============================================================================
   Copyright (c) 2014-2020 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/

#ifndef COMPOSER_SYNTHESIZER_H
#define COMPOSER_SYNTHESIZER_H

#include <q/support/literals.hpp>
#include <q/synth/sin.hpp>
#include <q_io/audio_stream.hpp>

namespace q = cycfi::q;
using namespace q::literals;

struct sin_synth : q::port_audio_stream
{
    sin_synth(q::frequency freq)
        : port_audio_stream( 0, 2)
        , phase(freq, this->sampling_rate())
    {}

    void process(out_channels const& out);

    void set(q::frequency freq, q::duration dur);

    q::phase_iterator   phase;       // The phase iterator
    q::duration         duration();  // Note duration
};

#endif // COMPOSER_SYNTHESIZER_H
