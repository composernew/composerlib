#include <array>
#include <audio/synthesizer.h>
#include <q/support/notes.hpp>

using namespace q::notes;

int main(){

    const int num_notes = 6;
    auto time = q::duration(2);

    // 6 string guitar frequencies:
    constexpr auto low_e   = E[2];
    constexpr auto a       = A[2];
    constexpr auto d       = D[3];
    constexpr auto g       = G[3];
    constexpr auto b       = B[3];
    constexpr auto high_e  = E[4];

    std::array<q::frequency,num_notes> notes = {low_e, a, d, g, b, high_e};

    sin_synth synth{notes[0]};
    synth.start();

    for(const q::frequency &note : notes){
        synth.set(note);
        q::sleep(time);
    }

    synth.stop();

    return 0;
}