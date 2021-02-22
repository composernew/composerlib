//
// Created by Carla on 03/02/2021.
//

#include <array>
#include <audio/synthesizer.h>

int main(){

    constexpr int n = 5;

    std::array<q::frequency,n> notes = {440_Hz, 330_Hz, 550_Hz, 100_Hz, 440_Hz};

    for(size_t i = 0; i < notes.size(); i++){
        sin_synth synth{notes[i]};

        synth.start();
        q::sleep(3_s);
        synth.stop();
    }

    return 0;
}