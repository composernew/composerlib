#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <midi/smf_io.h>
#include <MidiFile.h>

TEST_CASE ("Read SMF") {
    smf_io smf_reader;
    REQUIRE(smf_reader.read("../../resources/twinkle.midi") == true);
}