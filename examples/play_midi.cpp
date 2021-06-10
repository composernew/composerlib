#include <composer/synth.h>
#include <composer/midi_stream.h>
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

    smf::MidiFile midi_file;

    if (midi_file.read("../resources/twinkle.midi")){

        midi_file.doTimeAnalysis();

        if (midi_file.getTrackCount() > 1) {

            // Converts multi-track data into a single time sequence.
            // This makes possible to process the event list as a single track list
            midi_file.joinTracks();
        }
    }

    // Creates a stream to pass the MIDI Event List to the midi_processor
    midi_stream stream(midi_file[0]);

    // Creates a processor to play the MIDI Event List
    midi_processor processor{synth_};

    synth_.start();

    stream.process(processor);

    synth_.stop();

    return 0;
}