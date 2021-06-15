#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <composer/midi_stream.h>
#include <MidiFile.h>

using cycfi::q::literals::operator""_ms;
using cycfi::q::literals::operator""_s;
using cycfi::q::literals::operator""_dB;

using namespace composer;

TEST_CASE ("MidiEventList contains harmony") {

    // Empty MIDI file
    smf::MidiFile midi_file;

    REQUIRE(is_harmony(midi_file[0]) == false);

    // MIDI file containing melody
    midi_file.read("../resources/twinkle.midi");

    if (midi_file.getTrackCount() > 1) {
        midi_file.joinTracks();
    }

    REQUIRE(is_harmony(midi_file[0]) == false);

    // MIDI file containing harmony
    midi_file.read("../resources/twinkle_harmonic.mid");

    if (midi_file.getTrackCount() > 1) {
        midi_file.joinTracks();
    }

    REQUIRE(is_harmony(midi_file[0]));
}

TEST_CASE ("Dispatch MIDI messages to synthesizer") {

    // Test resources
    auto env_cfg = cycfi::q::envelope::config {
        100_ms    // attack rate
        , 1_s     // decay rate
        , -12_dB // sustain level
        , 5_s    // sustain rate
        , 1_s    // release rate
    };

    synth synth_(env_cfg);
    midi_processor processor(synth_);
    smf::MidiFile midi_file;

    REQUIRE_THROWS_WITH(dispatch_midi_messages(processor, midi_file[0]), "Empty MidiEventList.");

    // MIDI file containing melody
    midi_file.read("../resources/twinkle.midi");

    if (midi_file.getTrackCount() > 1) {
        midi_file.joinTracks();
    }

    REQUIRE_NOTHROW(dispatch_midi_messages(processor, midi_file[0]));

    midi_file.read("../resources/twinkle_harmonic.mid");

    if (midi_file.getTrackCount() > 1) {
        midi_file.joinTracks();
    }

    REQUIRE_THROWS_WITH(dispatch_midi_messages(processor, midi_file[0]), "This MIDI file contains harmonies: not implemented yet.");
}

TEST_CASE ("smf::MidiMessage to raw message") {

    smf::MidiMessage empty_message;

    REQUIRE_THROWS_WITH(to_raw_message(empty_message), "Empty message");

    // Valid message
    smf::MidiMessage message = {144, 48, 64};

    REQUIRE_NOTHROW(to_raw_message(message));
}