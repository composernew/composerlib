#include <audio/synthesizer.h>
#include <q/support/notes.hpp>

namespace q = cycfi::q;
using namespace q::notes;

int main(){

    constexpr int triangle = 1;
    constexpr int square   = 2;
    constexpr int saw      = 3;

    // 6 string guitar frequencies:
    constexpr auto low_e   = E[2];
    constexpr auto a       = A[2];
    constexpr auto d       = D[3];
    constexpr auto g       = G[3];
    constexpr auto b       = B[3];
    constexpr auto high_e  = E[4];

    std::vector<std::pair<int,q::frequency>> notes = {{10, low_e},{8, a},{6, d},
                                                      {4, g},{2, b},{0, high_e}};

    //default sine wave
    synth sine_synth{};
    sine_synth.start();
    sine_synth.play(notes);
    sine_synth.stop();

    //triangle
    synth triangle_synth{triangle};
    triangle_synth.start();
    triangle_synth.play(notes);
    triangle_synth.stop();

    //square
    synth square_synth{square};
    square_synth.start();
    square_synth.play(notes);
    square_synth.stop();

    //saw
    synth saw_synth{saw};
    saw_synth.start();
    saw_synth.play(notes);
    saw_synth.stop();

    return 0;
}