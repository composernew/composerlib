//
// Created by Alan Freitas on 12/14/20.
//

#include "melody.h"

namespace composer {

    std::default_random_engine melody::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    melody::melody(const size_t &problem_size, const size_t &individual_size)
    : melody_(problem_size)
    {
        std::vector<int> measure(individual_size);

        for (size_t i = 0; i < problem_size; ++i) {
            std::generate(measure.begin(), measure.end(), [this]() {
                std::uniform_int_distribution<int> distribution(0, 127);
                return distribution(this->generator_);
            });
            this->melody_[i] = measure;
        }
    }

    void melody::display() {

        std::cout << "Melody" << std::endl;

        for (const std::vector<int> &measure : this->melody_) {
            for (const int &note : measure) {
                std::cout << note << " ";
            }
            std::cout << std::endl;
        }
    }
}
