//
// Created by Carla on 03/02/2021.
//

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