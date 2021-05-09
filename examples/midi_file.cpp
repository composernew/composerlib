//
// Created by Carla on 04/05/2021.
//

#include <audio/synth.h>
#include <midi/smf_reader.h>
#include <midi/midi_stream.h>

using cycfi::q::literals::operator""_ms;
using cycfi::q::literals::operator""_s;
using cycfi::q::literals::operator""_dB;

int main() {

    auto env_cfg = cycfi::q::envelope::config {
            100_ms    // attack rate
            , 1_s     // decay rate
            , -12_dB // sustain level
            , 5_s    // sustain rate
            , 1_s    // release rate
    };

    synth synth_(env_cfg);
    midi_stream stream(smf_reader::read("../notes.midi"));
    midi_processor processor{synth_};

    synth_.start();

    stream.process(processor);

    synth_.stop();

    return 0;
}