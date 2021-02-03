//
// Created by Carla on 03/02/2021.
//

#include <notes/notes.h>

int main(){
    sin_synth synth{ 440_Hz };

    synth.start();
    q::sleep(5_s);
    synth.stop();
    return 0;
}