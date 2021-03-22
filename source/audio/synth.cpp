#include <audio/synth.h>

void synth::process(const out_channels &out) {

    auto left = out[0];
    auto right = out[1];
    for (auto frame : out.frames())
    {
        switch (timbre_) {
            case timbre::triangle:
                right[frame] = left[frame] = cycfi::q::triangle(phase++);
                break;
            case timbre::square:
                right[frame] = left[frame] = cycfi::q::square(phase++);
                break;
            case timbre::saw:
                right[frame] = left[frame] = cycfi::q::saw(phase++);
                break;
            default:
                right[frame] = left[frame] = cycfi::q::sin(phase++);
        }
    }
}

void synth::set(const cycfi::q::frequency &freq, const cycfi::q::duration &dur) {
    phase.set(freq, this->sampling_rate());
    cycfi::q::sleep(dur);
}

void synth::play(std::vector<std::tuple<int, cycfi::q::frequency,cycfi::q::duration>> &notes){

    std::sort(notes.begin(),notes.end());

    for(size_t i = 1; i <= notes.size(); ++i){

        // plays the frequency with specified duration
        set(std::get<1>(notes[i-1]), std::get<2>(notes[i-1]));
    }
}

void synth::set_timbre(timbre timbre) {
    timbre_ = timbre;
}