//
// Created by Alan Freitas on 12/14/20.
//

#include "melody.h"
#include "melody_problem.h"
#include <utility>
#include <range/v3/all.hpp>

namespace composer {

    std::default_random_engine melody::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    melody::melody(melody_problem const &problem)
        : melody_(problem.get_melody()),
          rhythm(problem.get_rhythm())
    {
        composer::melody::evaluate(); // Initialize valence and arousal
        this->distance = composer::melody::euclidean_distance(problem.get_target(), this->valence_arousal);
    }

    melody::melody() {
        this->melody_ = {};
        this->valence_arousal = {0.,0.};
        this->distance = 0;
        this->rhythm = 0;
    }

    double melody::evaluate_pitch_distribution() {

        int mode = 0;
        int max_count = 1; // Avoids that mode became the first element of the individual when there is no mode.
        std::vector<int> counts;

        for (const auto &value : this->melody_) {

            auto count = static_cast<int>(ranges::count(this->melody_.begin(),
                                           this->melody_.end(), value));
            counts.emplace_back(count);

            if (count > max_count) {
                max_count = count;
                mode = std::get<0>(value);
            }
        }

        std::sort(counts.begin(), counts.end(), std::greater<>());
        auto count_mode = static_cast<int>(ranges::count(counts.begin(),
                                                    counts.end(),
                                                    counts[0]));

        if (count_mode > counts[0] ||
            mode == static_cast<int>(melody_problem::feature_type::pause)) mode = 0;

        return mode;
    }

    double melody::evaluate_pitch_variety() const {

        std::vector<std::tuple<int,int,int>> melody_sorted(this->melody_.size());

        ranges::partial_sort_copy(this->melody_.begin(), this->melody_.end(),
                               melody_sorted.begin(), melody_sorted.end());

        auto unique_values = static_cast<double>(ranges::unique(melody_sorted.begin(),
                                    melody_sorted.end()) - melody_sorted.begin());

        // Empty melody
        if ((unique_values == 1) && std::get<0>(melody_sorted[0]) ==
            static_cast<int>(melody_problem::feature_type::pause)) unique_values = 0;

        return unique_values;
    }

    double melody::evaluate_average_volume() {

        double count = 0.;

        for (const auto &value : this->melody_) {

           count += static_cast<double>(std::get<2>(value));
        }

        return count/static_cast<double>(this->melody_.size());
    }

    double melody::normalize(double value, double max, double min, double max_value, double min_value) {
        return (max - min) * (value - min_value)/(max_value - min_value) + min;
    }

    void melody::set_valence_arousal(std::pair<double,double> new_valence_arousal) {
        this->valence_arousal = new_valence_arousal;
    }

    void melody::evaluate() {

        auto max_value = static_cast<double>(this->melody_.size());
        double normalized_pitch_distribution;
        double normalized_pitch_variety;
        double normalized_rhythm;
        double normalized_average_volume;

        double pitch_variety = melody::evaluate_pitch_variety();

        pitch_variety == 0 ? normalized_pitch_variety = 0 :
                           normalized_pitch_variety = melody::normalize(
                   pitch_variety, 1, -1, max_value, 1.);

        double pitch_distribution = melody::evaluate_pitch_distribution();

        pitch_distribution <
                static_cast<double>(melody_problem::feature_type::pause) + 1.
            ? normalized_pitch_distribution = 0
            : normalized_pitch_distribution = melody::normalize(
                  pitch_distribution, 1., -1.,
                  static_cast<double>(
                      melody_problem::feature_type::highest_pitch),
                  static_cast<double>(melody_problem::feature_type::pause) +
                      1.);

        normalized_rhythm = melody::normalize(
            this->rhythm, 1., -1.,
            static_cast<double>(melody_problem::feature_type::fastest_tempo),
            static_cast<double>(melody_problem::feature_type::slowest_tempo));

        double average_volume = evaluate_average_volume();

        average_volume == 0
            ? normalized_average_volume = 0
            : normalized_average_volume = melody::normalize(
                  average_volume, 1., -1.,
                  static_cast<double>(
                      melody_problem::feature_type::highest_volume),
                  static_cast<double>(
                      melody_problem::feature_type::lowest_volume));

        double valence = normalized_pitch_variety;
        double arousal =
            (normalized_pitch_distribution + normalized_rhythm + normalized_average_volume) / 3.;

        melody::set_valence_arousal({valence, arousal});
    }

    double melody::euclidean_distance(std::pair<double,double> const &p1, std::pair<double,double> const &p2) {
        return sqrt(pow((p1.first - p2.first), 2) + (pow((p1.second - p2.second),2)));
    }

    void melody::simple_mutation() {

        auto max = static_cast<int>(this->melody_.size()-1);

        std::uniform_int_distribution up_down(0, 1);
        std::uniform_int_distribution d(0, max);

        int position = d(generator_);

        if(up_down(generator_)) {
            if (std::get<0>(this->melody_[position]) <= static_cast<int>(melody_problem::feature_type::highest_pitch)-2)
                std::get<0>(this->melody_[position]) = std::get<0>(this->melody_[position]) + 2;

            if(std::get<2>(this->melody_[position]) < static_cast<int>(melody_problem::feature_type::highest_volume))
                ++std::get<2>(this->melody_[position]);
        }
        else {
            if (std::get<0>(this->melody_[position]) >= static_cast<int>(melody_problem::feature_type::pause)+2)
                std::get<0>(this->melody_[position]) = std::get<0>(this->melody_[position]) - 2;

            if(std::get<2>(this->melody_[position]) > static_cast<int>(melody_problem::feature_type::lowest_volume))
                --std::get<2>(this->melody_[position]);
        }
    }

    void melody::reverse_measure() {
        ranges::reverse(this->melody_.begin(), this->melody_.end());
    }

    void melody::exchange_pulses() {

        auto max = static_cast<int>(this->melody_.size()-1);

        std::uniform_int_distribution d(0, max);
        int first_pulse = d(generator_);

        int second_pulse = d(generator_);

        while (second_pulse == first_pulse) {
            second_pulse = d(generator_);
        }

        std::swap(this->melody_[first_pulse], this->melody_[second_pulse]);
    }

    void melody::reverse_pulses() {

        auto max = static_cast<int>(this->melody_.size()-1);
        std::uniform_int_distribution d(0, (max - 1));

        int first_pulse = d(generator_);
        d = std::uniform_int_distribution((first_pulse+1), max);

        int second_pulse = d(generator_);
        std::reverse(this->melody_.begin() + first_pulse, this->melody_.begin() + (second_pulse+1));
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

        if (d(generator_)) {
            child.set_rhythm(first_parent.get_rhythm());
        }
        else {
            child.set_rhythm(second_parent.get_rhythm());
        }

        return child;
    }

    std::vector<std::tuple<int,int,int>> melody::get_melody() const {
        return this->melody_;
    }

    double melody::get_distance() const {
        return this->distance;
    }

    std::pair<double, double> melody::get_valence_arousal() const {
        return this->valence_arousal;
    }

    [[maybe_unused]] void melody::set_valence_arousal(double valence, double arousal) {
        this->valence_arousal.first = valence;
        this->valence_arousal.second = arousal;
    }

    void melody::set_melody(const std::tuple<int,int,int> &note_value) {
        this->melody_.emplace_back(note_value);
    }

    void melody::set_distance(double new_distance) {
        this->distance = new_distance;
    }

    void melody::set_rhythm(double new_rhythm) {
        this->rhythm = new_rhythm;
    }

    double melody::get_rhythm() const {
        return this->rhythm;
    }
}
