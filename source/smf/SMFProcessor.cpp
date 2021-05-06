//
// Created by Carla on 04/05/2021.
//

#include "SMFProcessor.h"
#include <MidiFile.h>
#include <cmath>

void SMFProcessor::process(const std::string &filename) {

    synth_.start();
    smf::MidiFile midiFile;

    midiFile.read(filename);

    midiFile.doTimeAnalysis();
    midiFile.linkNotePairs();

    int tracks = midiFile.getTrackCount();

    double duration;
    double frequency;

    for(int track = 0; track < tracks; track++){
        for(int event = 0; event < midiFile[track].size(); event++){
            if(midiFile[track][event].isNoteOn()){

                duration = midiFile[track][event].getDurationInSeconds();

                //Calculates the frequency based on MIDI note number
                frequency = 440. * std::pow(2,((static_cast<double>(midiFile[track][event][1])-69.)/12.));
                synth_.set(frequency,duration);
            }
        }
    }

    synth_.stop();
}
