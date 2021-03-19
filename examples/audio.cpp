#include <audio/synth.h>
#include <q/support/notes.hpp>

int main(){

    enum class timbre : int {sine = 0, triangle, square, saw};

    // 6 string guitar frequencies:
    constexpr auto low_e   = cycfi::q::notes::E[2];
    constexpr auto a       = cycfi::q::notes::A[2];
    constexpr auto d       = cycfi::q::notes::D[3];
    constexpr auto g       = cycfi::q::notes::G[3];
    constexpr auto b       = cycfi::q::notes::B[3];
    constexpr auto high_e  = cycfi::q::notes::E[4];

    std::vector<std::pair<int,cycfi::q::frequency>> notes = {{10, low_e},{8, a},{6, d},
                                                      {4, g},{2, b},{0, high_e}};

    synth _synth{timbre::sine};
    _synth.start();
    _synth.play(notes);
    _synth.stop();

    //triangle
    _synth.set_timbre(static_cast<int>(timbre::triangle));
    _synth.start();
    _synth.play(notes);
    _synth.stop();

    //square
    _synth.set_timbre(static_cast<int>(timbre::square));
    _synth.start();
    _synth.play(notes);
    _synth.stop();

    //saw
    _synth.set_timbre(static_cast<int>(timbre::saw));
    _synth.start();
    _synth.play(notes);
    _synth.stop();

    return 0;
}