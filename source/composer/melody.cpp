//
// Created by Alan Freitas on 12/14/20.
//

#include "melody.h"
#include <iostream>
#include <chrono>

namespace composer {

    std::default_random_engine melody::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    melody::melody(smf::MidiEventList &notes)
        : notes_(notes) {}

    const smf::MidiEventList &melody::notes() const { return notes_; }

    void melody::disp() {

        std::cout << "Solution" << std::endl;

        for (int event = 0; event < notes_.size(); ++event) {

            std::cout << std::dec << notes_[event].tick;
            std::cout << '\t' << std::dec << notes_[event].seconds;
            std::cout << '\t';

            if (notes_[event].isNoteOn())
                std::cout << notes_[event].getDurationInSeconds();
            std::cout << '\t' << std::hex;

            for (int i=0; i< notes_[event].size(); i++)
                std::cout << (int)notes_[event][i] << ' ';
            std::cout << std::endl;
        }
    }

    void melody::mutation(double mutation_strength) {

        std::uniform_real_distribution<double> d(0.0,1.0);

        for (int event = 0; event < notes_.size(); ++event) {

            if(notes_[event].size() == 3){

                if (d(generator_) < mutation_strength && (notes_[event][1] + 1) < 127) {
                    notes_[event][1] = notes_[event][1] + 1;
                }
            }
        }
    }
}

