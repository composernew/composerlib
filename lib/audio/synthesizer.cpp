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
        // Generate the ADSR envelope
        auto env_ = env();

        // Set the filter frequency
        filter.cutoff(env_);

        // Synthesize the sin wave
        auto val = q::sin(phase++);

        // Apply the envelope (amplifier and filter) with soft clip
        val = clip(filter(val) * env_);

        //Output
        right[frame] = left[frame] = val;
    }
}