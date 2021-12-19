//
// Created by Alan Freitas on 12/14/20.
//

#include "melody.h"

namespace composer {

    std::default_random_engine melody::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    melody::melody(const std::vector<int> &initial_melody)
    : melody_(initial_melody.size())
    {
        melody_ = initial_melody;
        distance = 0;
    }

    void melody::display(std::vector<std::vector<int>> const &solution) {

        for (const auto &melody : solution) {

            for (const int &note : melody) {
                std::cout << note << " ";
            }
            std::cout << std::endl;
        }
    }

    std::vector<int> melody::create_individual(const size_t &individual_size) {
        std::vector<int> measure(individual_size);

        std::ranges::generate(measure.begin(), measure.end(), []() {
            std::uniform_int_distribution d(21, 108);
            return d(generator_);
        });

        return measure;
    }

    std::vector<int> melody::crossover(const std::vector<int> &first_parent, const std::vector<int> &second_parent) {
        std::uniform_int_distribution d(0,1);
        std::vector<int> child;

        for (size_t i = 0; i < first_parent.size(); ++i) {
            if (d(generator_)) {
                child.push_back(first_parent[i]);
            }
            else {
                child.push_back(second_parent[i]);
            }
        }

        return child;
    }

    void melody::simple_mutation(std::vector<int> &individual) {

        std::uniform_int_distribution dist_int(0, 1);

        for (int &item : individual) {
            if(dist_int(generator_)) {
                if (item < 108)
                    item = item + 1;
            }
            else {
                if (item > 21)
                    item = item - 1;
            }
        }
    }

    void melody::reverse_measure(std::vector<int> &individual) {
        std::ranges::reverse(individual.begin(), individual.end());
    }

    void melody::exchange_pulses(std::vector<int> &individual) {

        std::uniform_int_distribution value_distribution(0, static_cast<int>(individual.size()-1));
        int first_pulse = value_distribution(generator_);
        int second_pulse = value_distribution(generator_);
        std::swap(individual[first_pulse], individual[second_pulse]);
    }

    void melody::reverse_pulses(std::vector<int> &individual) {

        std::uniform_int_distribution first_value_distribution(0, static_cast<int>(individual.size()));
        int first_pulse = first_value_distribution(generator_);

        std::uniform_int_distribution second_value_distribution(first_pulse, static_cast<int>(individual.size()));
        int second_pulse = second_value_distribution(generator_);

        std::reverse(individual.begin() + first_pulse, individual.begin() + second_pulse);
    }

    double melody::normalize(double value, double max, double min, double max_value, double min_value) {
        return (max - min) * (value - min_value)/(max_value - min_value) + min;
    }

    double melody::evaluate_pitch_distribution(const std::vector<int> &individual) {

        int mode = 0;
        int max_count = 0;

        for (const auto &value : individual) {
            int count = std::ranges::count(individual.begin(), individual.end(), value);
            if (count > max_count) {
                max_count = count;
                mode = value;
            }
        }

        return mode;
    }

    double melody::evaluate_pitch_variety(std::vector<int> individual) {

        double unique_values;

        std::ranges::sort(individual.begin(), individual.end());
        unique_values = std::unique(individual.begin(), individual.end()) - individual.begin();

        return unique_values;
    }

    std::tuple<double, double, double, double> melody::evaluate(const std::vector<int> &individual, double max_value) {
        double valence;
        double arousal;
        double normalized_pitch_variety =
            normalize(evaluate_pitch_variety(individual), 1,
                      -1, max_value, 1.);
        double normalized_pitch_distribution =
            normalize(evaluate_pitch_distribution(individual), 1.,
                      -1., 108., 21);

        valence = normalized_pitch_variety;
        arousal =  (normalized_pitch_variety + normalized_pitch_distribution)/2.;

        return {std::min(0.,valence), std::min(0.,arousal), std::max(0.,valence), std::max(0.,arousal)};
    }

    std::vector<int> melody::get_melody() {
        return this->melody_;
    }

    void melody::clear() {
        melody_.clear();
    }

    void melody::set_distance(double new_distance) {
        this->distance = new_distance;
    }

    void melody::set_valence_arousal(
        const std::pair<double, double> new_valence_arousal) {
        this->valence_arousal = new_valence_arousal;
    }

    std::pair<double, double> melody::get_valence_arousal() {
        return this->valence_arousal;
    }

    melody::melody() {
        this->melody_ = {};
        this->valence_arousal = {0.0, 0.0};
        this->distance = 0.0;
    }

    double melody::get_distance() {
        return this->distance;
    }
}
