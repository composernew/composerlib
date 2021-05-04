//
// Created by Carla on 04/05/2021.
//

#include "smf_processor.h"
#include <MidiFile.h>

void SMFProcessor::process(std::string filename) {

    smf::MidiFile midiFile;

    midiFile.read(filename);

    midiFile.doTimeAnalysis();
    midiFile.linkNotePairs();

    int tracks = midiFile.getTrackCount();
    std::cout << "TPQ: " << midiFile.getTicksPerQuarterNote() << std::endl;

    if(tracks > 1) std::cout << "TRACKS " << tracks << std::endl;

    for(int track = 0; track < tracks; track++){
        if(tracks > 1) std::cout << "\nTrack " << track << std::endl;
        std::cout << "Tick\tSeconds\tDur\tMessage" << std::endl;

        for(int event = 0; event < midiFile[track].size(); event++){
            std::cout << std::dec << midiFile[track][event].tick;
            std::cout << '\t' << std::dec << midiFile[track][event].seconds;
            std::cout << '\t';

            if(midiFile[track][event].isNoteOn())
                std::cout << midiFile[track][event].getDurationInSeconds();
            std::cout << '\t' << std::hex;

            for(int i = 0; i < midiFile[track][event].size(); i++){
                std::cout << (int)midiFile[track][event][i] << ' ';
            }
            std::cout << std::endl;
        }
    }
}
