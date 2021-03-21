#include <audio/synth.h>
#include <q/support/notes.hpp>

int main(){

    // 6 string guitar frequencies:
    constexpr auto low_e   = cycfi::q::notes::E[2];
    constexpr auto a       = cycfi::q::notes::A[2];
    constexpr auto d       = cycfi::q::notes::D[3];
    constexpr auto g       = cycfi::q::notes::G[3];
    constexpr auto b       = cycfi::q::notes::B[3];
    constexpr auto high_e  = cycfi::q::notes::E[4];

    std::vector<std::pair<int,cycfi::q::frequency>> notes = {{10, low_e},{8, a},{6, d},
                                                      {4, g},{2, b},{0, high_e}};

    // Synthesizer timbres

    // sine timbre by default
    synth _synth{};
    _synth.start();
    _synth.play(notes);
    _synth.stop();

    _synth.set_timbre(synth::timbre::triangle);
    _synth.start();
    _synth.play(notes);
    _synth.stop();

    _synth.set_timbre(synth::timbre::square);
    _synth.start();
    _synth.play(notes);
    _synth.stop();

    _synth.set_timbre(synth::timbre::saw);
    _synth.start();
    _synth.play(notes);
    _synth.stop();

    return 0;
}