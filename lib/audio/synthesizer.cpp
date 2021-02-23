/*=============================================================================
   Copyright (c) 2014-2020 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/

#include <audio/synthesizer.h>

void sin_synth::process(const out_channels &out) {
    auto left = out[0];
    auto right = out[1];
    for (auto frame : out.frames())
    {
        // Synthesize the sin wave
        right[frame] = left[frame] = q::sin(phase++);
    }
}