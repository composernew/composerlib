#include <audio/synth.h>

void synth::process(const out_channels &out) {

    auto left = out[0];
    auto right = out[1];
    for (auto frame : out.frames())
    {
        switch (timbre_) {
            case 1:
                right[frame] = left[frame] = cycfi::q::triangle(phase++);
                break;
            case 2:
                right[frame] = left[frame] = cycfi::q::square(phase++);
                break;
            case 3:
                right[frame] = left[frame] = cycfi::q::saw(phase++);
                break;
            default:
                right[frame] = left[frame] = cycfi::q::sin(phase++);
        }
    }
}

void synth::set(const cycfi::q::frequency &freq, const int &dur) {
    phase.set(freq, this->sampling_rate());
    cycfi::q::sleep(dur);
}

void synth::play(std::vector<std::pair<int, cycfi::q::frequency>> &notes){

    std::sort(notes.begin(),notes.end());

    int duration;

    for(int i = 1; i < notes.size(); ++i){

        duration = notes[i].first - notes[i-1].first;

        set(notes[i-1].second,duration);
    }

    duration = 3;
    set(notes.back().second,duration);
}

void synth::set_timbre(int timbre) {
    timbre_ = timbre;
}