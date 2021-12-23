//
// Created by Alan Freitas on 12/14/20.
//

#include "melody.h"
#include "melody_problem.h"

#include <utility>

namespace composer {

    std::default_random_engine melody::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    melody::melody(melody_problem const &problem) {
        this->melody_ = problem.get_melody();
        this->distance = composer::melody::euclidean_distance(problem.get_target(), this->valence_arousal);
        composer::melody::evaluate(); // Initialize valence and arousal
    }

    melody::melody() {
        this->valence_arousal = {0.,0.};
        this->distance = 0;
    }

    double melody::evaluate_pitch_distribution() {

        int mode = 0;
        int max_count = 1; // Avoids that mode became the first element of the individual when there is no mode.

        for (const auto &value : this->melody_) {
            auto count = static_cast<int>(std::ranges::count(this->melody_.begin(),
                                           this->melody_.end(), value));
            if (count > max_count) {
                max_count = count;
                mode = value;
            }
        }

        return mode;
    }

    double melody::evaluate_pitch_variety() {

        double unique_values;

        std::ranges::sort(this->melody_.begin(),this->melody_.end());
        unique_values = std::unique(this->melody_.begin(),
                                    this->melody_.end()) - this->melody_.begin();

        return unique_values;
    }

    double melody::normalize(double value, double max, double min, double max_value, double min_value) {
        return (max - min) * (value - min_value)/(max_value - min_value) + min;
    }

    void melody::set_valence_arousal(std::pair<double,double> new_valence_arousal) {
        this->valence_arousal = new_valence_arousal;
    }

    void melody::evaluate() {

        auto max_value = static_cast<double>(this->melody_.size());

        double normalized_pitch_variety = melody::normalize(
            melody::evaluate_pitch_variety(), 1, -1, max_value, 1.);
        double normalized_pitch_distribution = melody::normalize(
            melody::evaluate_pitch_distribution(), 1., -1., 108., 20);

        double valence = normalized_pitch_variety;
        double arousal =
            (normalized_pitch_variety + normalized_pitch_distribution) / 2.;

        melody::set_valence_arousal({valence, arousal});
    }

    double melody::euclidean_distance(std::pair<double,double> p1, std::pair<double,double> p2) {
        return sqrt(pow((p1.first - p2.first), 2) + (pow((p1.second - p2.second),2)));
    }

    void melody::simple_mutation() {

        std::uniform_int_distribution dist_int(0, 1);

        for (int &item : this->melody_) {
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

    void melody::reverse_measure() {
        std::ranges::reverse(this->melody_.begin(), this->melody_.end());
    }

    void melody::exchange_pulses() {

        int max = static_cast<int>(this->melody_.size()-1);

        std::uniform_int_distribution value_distribution(0, max);
        int first_pulse = value_distribution(generator_);
        int second_pulse = value_distribution(generator_);
        std::swap(this->melody_[first_pulse], this->melody_[second_pulse]);
    }

    void melody::reverse_pulses() {

        std::uniform_int_distribution first_value_distribution(0, static_cast<int>(this->melody_.size()));
        int first_pulse = first_value_distribution(generator_);

        std::uniform_int_distribution second_value_distribution(first_pulse, static_cast<int>(this->melody_.size()));
        int second_pulse = second_value_distribution(generator_);

        std::reverse(this->melody_.begin() + first_pulse, this->melody_.begin() + second_pulse);
    }

    melody melody::crossover(const melody &first_parent, const melody &second_parent) {
        std::uniform_int_distribution d(0,1);
        melody child;

        for (size_t i = 0; i < first_parent.get_melody().size(); ++i) {
            if (d(generator_)) {
                child.set_melody(first_parent.get_melody()[i]);
            }
            else {
                child.set_melody(second_parent.get_melody()[i]);
            }
        }

        return child;
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

    void melody::set_melody(int note) {
        this->melody_.emplace_back(note);
    }
}
