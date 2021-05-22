//
// Created by Carla on 04/05/2021.
//

#include <audio/synth.h>
#include <midi/midi_stream.h>
#include <midi/smf_io.h>
#include <MidiFile.h>

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

    smf::MidiFile midiFile = smf_io::read("../resources/twinkle.midi");
    midi_stream stream(midiFile[0]);
    midi_processor processor{synth_};

    synth_.start();

    stream.process(processor);

    synth_.stop();

    smf_io::write(midiFile, "midiFile.midi");

    return 0;
}