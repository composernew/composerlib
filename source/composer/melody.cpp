//
// Created by Alan Freitas on 12/14/20.
//

#include "melody.h"
#include <iostream>
#include <chrono>

namespace composer {

    std::default_random_engine melody::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    melody::melody(std::vector<std::vector<int>> &notes)
        : melody_(notes) {}

    const std::vector<std::vector<int>> &melody::notes() const { return melody_; }

    void melody::disp() {

        std::cout << "Solution" << std::endl;

        for (const std::vector<int> &measure : melody_) {
            for (const int &note : measure) {
                std::cout << note << " ";
            }
            std::cout << std::endl;
        }
    }

    void melody::mutation(double mutation_strength) {

        std::uniform_real_distribution<double> d(0.0,1.0);

        for (int event = 0; event < melody_.size(); ++event) {

            if(melody_[event].size() == 3){

                if (d(generator_) < mutation_strength && (melody_[event][1] + 1) < 127) {
                    melody_[event][1] = melody_[event][1] + 1;
                }
            }
        }
    }
}

