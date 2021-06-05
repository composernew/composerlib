#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <midi/smf_io.h>
#include <MidiFile.h>

TEST_CASE ("Write SMF") {
    smf::MidiFile midiFile;
    REQUIRE(smf_io::write(midiFile,"midiFile.midi") == true);
}
