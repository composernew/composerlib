//
// Created by Carla on 04/05/2021.
//

#include "SMFReader.h"
#include <MidiFile.h>
#include <cmath>

void SMFReader::read(const std::string &filename) {

    smf::MidiFile midiFile;

    midiFile.read(filename);

    midiFile.doTimeAnalysis();
    midiFile.linkNotePairs();

    double duration;
    double frequency;

    for(int event = 0; event < midiFile[0].size(); event++){
        if(midiFile[0][event].isNoteOn()){

            duration = midiFile[0][event].getDurationInSeconds();

            //Calculates the frequency based on MIDI note number
            frequency = 440. * std::pow(2,((static_cast<double>(midiFile[0][event][1])-69.)/12.));

            std::tuple<int,cycfi::q::frequency,cycfi::q::duration> note = {event,frequency,duration};

            notes_.emplace_back(note);
        }
    }
}
