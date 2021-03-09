#include <audio/synthesizer.h>

void synth::process(const out_channels &out) {

    auto left = out[0];
    auto right = out[1];
    for (auto frame : out.frames())
    {
        switch (_timbre) {
            case 1:
                right[frame] = left[frame] = q::triangle(phase++);
                break;
            case 2:
                right[frame] = left[frame] = q::square(phase++);
                break;
            case 3:
                right[frame] = left[frame] = q::saw(phase++);
                break;
            default:
                right[frame] = left[frame] = q::sin(phase++);
        }
    }
}

void synth::set(const q::frequency &freq, const int &dur) {
    phase.set(freq, this->sampling_rate());
    q::sleep(dur);
}

void synth::play(std::vector<std::pair<int, q::frequency>> &notes){

    std::sort(notes.begin(),notes.end());

    int duration;

    for(int i = 1; i < notes.size(); ++i){

        duration = notes[i].first - notes[i-1].first;

        set(notes[i-1].second,duration);
    }

    duration = 3;
    set(notes.back().second,duration);
}