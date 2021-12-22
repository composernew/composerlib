//
// Created by Carla on 21/12/2021.
//

#include "melody_problem.h"

namespace composer {

    melody_problem::melody_problem(std::pair<double,double> target)
        : target_(target)
    {
        this->melody_ = c_major_double();
    }

    std::vector<int> melody_problem::c_major_double() {
        return {60, 62, 64, 65, 67, 69, 71, 72, 60, 62, 64, 65, 67, 69, 71, 72};
    }

    std::vector<int> composer::melody_problem::get_melody() const {
        return this->melody_;
    }

    double melody_problem::evaluate_pitch_distribution(melody_problem const &individual) {

        int mode = 0;
        int max_count = 1; // Avoids that mode became the first element of the individual when there is no mode.

        for (const auto &value : individual.get_melody()) {
            auto count = static_cast<int>(std::ranges::count(individual.get_melody().begin(),
                                                             individual.get_melody().end(), value));
            if (count > max_count) {
                max_count = count;
                mode = value;
            }
        }

        return mode;
    }

    double melody_problem::evaluate_pitch_variety(melody_problem const &individual) {

        double unique_values;

        std::ranges::sort(individual.get_melody().begin(),individual.get_melody().end());
        unique_values = std::unique(individual.get_melody().begin(),
                                    individual.get_melody().end()) - individual.get_melody().begin();

        return unique_values;
    }

    double melody_problem::normalize(double value, double max, double min, double max_value, double min_value) {
        return (max - min) * (value - min_value)/(max_value - min_value) + min;
    }

    std::pair<double,double> melody_problem::evaluate(melody_problem const &individual){

        auto max_value = static_cast<double>(individual.get_melody().size());

        double normalized_pitch_variety =
            melody_problem::normalize(melody_problem::evaluate_pitch_variety(individual), 1,
                              -1, max_value, 1.);
        double normalized_pitch_distribution =
            melody_problem::normalize(melody_problem::evaluate_pitch_distribution(individual), 1.,
                              -1., 108., 20);

        double valence = normalized_pitch_variety;
        double arousal = (normalized_pitch_variety + normalized_pitch_distribution)/2.;

        return {valence,arousal};
    }
    std::pair<double, double> melody_problem::get_target() const {
        return this->target_;
    }

} // namespace composer

