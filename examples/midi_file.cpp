//
// Created by Carla on 04/05/2021.
//

#include <smf/SMFProcessor.h>
#include <audio/synth.h>

int main() {

    synth synth;
    SMFProcessor processor(synth);

    processor.process("../notes.midi");

    return 0;
}