//
// Created by Carla on 04/05/2021.
//

#include <audio/synth.h>
#include <smf/SMFReader.h>

int main() {

    synth synth_;
    SMFReader reader;

    reader.read("../notes.midi");

    synth_.start();
    synth_.play(reader.notes_);
    synth_.stop();

    return 0;
}