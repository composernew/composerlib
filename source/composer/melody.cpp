//
// Created by Alan Freitas on 12/14/20.
//

#include "melody.h"
#include "melody_problem.h"

#include <utility>

namespace composer {

    melody::melody(melody_problem const &problem, const std::pair<double,double> &target)
        : target_(target)
    {
        this->valence_arousal = composer::melody_problem::evaluate(problem);
        this->distance = composer::melody::euclidean_distance(target, this->valence_arousal);
        this->melody_ = problem.get_melody();
    }

    melody::melody() {
        this->valence_arousal = {0.,0.};
        this->distance = 0;
    }

    double melody::evaluate_pitch_distribution(melody const &individual) {

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

    double melody::evaluate_pitch_variety(melody const &individual) {

        double unique_values;

        std::ranges::sort(individual.get_melody().begin(),individual.get_melody().end());
        unique_values = std::unique(individual.get_melody().begin(),
                                    individual.get_melody().end()) - individual.get_melody().begin();

        return unique_values;
    }

    double melody::normalize(double value, double max, double min, double max_value, double min_value) {
        return (max - min) * (value - min_value)/(max_value - min_value) + min;
    }

    std::pair<double,double> melody::evaluate(melody const &individual){

        auto max_value = static_cast<double>(individual.get_melody().size());

        double normalized_pitch_variety =
            melody::normalize(melody::evaluate_pitch_variety(individual), 1,
                              -1, max_value, 1.);
        double normalized_pitch_distribution =
            melody::normalize(melody::evaluate_pitch_distribution(individual), 1.,
                              -1., 108., 20);

        double valence = normalized_pitch_variety;
        double arousal = (normalized_pitch_variety + normalized_pitch_distribution)/2.;

        return {valence,arousal};
    }

    double melody::euclidean_distance(std::pair<double,double> p1, std::pair<double,double> p2) {
        return sqrt(pow((p1.first - p2.first), 2) + (pow((p1.second - p2.second),2)));
    }

    std::vector<int> melody::get_melody() const {
        return this->melody_;
    }

    double melody::get_distance() const {
        return this->distance;
    }

    std::pair<double, double> melody::get_valence_arousal() const {
        return this->valence_arousal;
    }
}
