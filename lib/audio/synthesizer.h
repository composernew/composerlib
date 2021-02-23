/*=============================================================================
   Copyright (c) 2014-2020 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/

#ifndef COMPOSER_SYNTHESIZER_H
#define COMPOSER_SYNTHESIZER_H

#include <q/support/literals.hpp>
#include <q/synth/sin.hpp>
#include <q_io/audio_stream.hpp>
#include <q/synth/envelope.hpp>

namespace q = cycfi::q;
using namespace q::literals;

struct sin_synth : q::port_audio_stream
{
    sin_synth(q::envelope::config env_cfg, int device_id)
        : port_audio_stream(q::audio_device::get(device_id), 0, 2)
        , env(env_cfg, this->sampling_rate())
    {}

    void process(out_channels const& out);

    q::phase_iterator phase;   // The phase iterator
    q::envelope       env;     // The envelope
};

#endif // COMPOSER_SYNTHESIZER_H
