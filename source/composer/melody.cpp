//
// Created by Alan Freitas on 12/14/20.
//

#include "melody.h"

namespace composer {

    std::default_random_engine melody::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    melody::melody(const size_t &problem_size, const size_t &individual_size)
    : melody_(problem_size)
    {
        for (size_t i = 0; i < problem_size; ++i) {
            this->melody_[i] = create_measure(individual_size);
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

    std::vector<int> melody::create_measure(const size_t &individual_size) {
        std::vector<int> measure(individual_size);

        std::generate(measure.begin(), measure.end(), []() {
            std::uniform_int_distribution<int> d(0, 127);
            return d(generator_);
        });

        return measure;
    }

    std::vector<int> melody::crossover(std::vector<int> &parent) {
        std::uniform_int_distribution<int> d(0,1);
        std::vector<int> child = create_measure(parent.size());

        for (size_t i = 0; i < parent.size(); ++i) {
            if (d(generator_)) {
                child[i] = parent[i];
            }
        }

        return child;
    }

    void melody::mutation(std::vector<int> &individual, double &mutation_strength) {

        std::uniform_real_distribution<double> d(0.0, 1.0);

        for (int &item : individual) {
            if (d(generator_) < mutation_strength) {
                item = item + 1;
            }
        }
    }

    std::tuple<double, double> melody::evaluate(std::vector<int> &individual) {
        double valence = 0;
        double arousal = 0;
        return {valence, arousal};
    }
}
