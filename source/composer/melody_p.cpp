//
// Created by Carla on 01/08/2021.
//

#include "melody_p.h"

namespace composer {

    std::default_random_engine melody_p::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    melody_p::melody_p(const size_t &problem_size, const size_t &individual_size)
        : melody_(problem_size)
    {
        std::uniform_int_distribution<int> d(0, 127);
        std::vector<int> measure(individual_size);

        for (size_t i = 0; i < problem_size; ++i) {
            std::generate(measure.begin(), measure.end(),
                          [d]() { return d(composer::melody_p::generator_);});
            this->melody_[i] = measure;
        }
    }
}
