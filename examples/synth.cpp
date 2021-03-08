#include <array>
#include <audio/synthesizer.h>
#include <q/support/notes.hpp>

using namespace q::notes;

int main(){

    const int num_notes = 6;

    // 6 string guitar frequencies:
    constexpr auto low_e   = E[2];
    constexpr auto a       = A[2];
    constexpr auto d       = D[3];
    constexpr auto g       = G[3];
    constexpr auto b       = B[3];
    constexpr auto high_e  = E[4];

    std::array<q::frequency,num_notes> notes = {low_e, a, d, g, b, high_e};
    std::array<q::duration,num_notes> times = {1,2,3,2,1,4};

    sin_synth synth{notes[0]};
    synth.start();

    for(int i = 0; i < notes.size(); ++i){
        synth.set(notes[i],times[i]);
    }

    synth.stop();

    return 0;
}