//
// Created by Carla on 21/12/2021.
//

#include "melody_problem.h"
#include <sstream>
#include <iostream>
#include <utility>

namespace composer {

    std::default_random_engine melody_problem::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    std::vector<std::tuple<int,int,int>> melody_problem::c_major_double() {
        return {{60, 50, 80},
                {62, 50, 80},
                {64, 50, 80},
                {65, 50, 80},
                {67, 50, 80},
                {69, 50, 80},
                {71, 50, 80},
                {72, 50, 80},
                {60, 50, 80},
                {62, 50, 80},
                {64, 50, 80},
                {65, 50, 80},
                {67, 50, 80},
                {69, 50, 80},
                {71, 50, 80},
                {72, 50, 80}};
    }

    std::vector<std::tuple<int,int,int>> melody_problem::twinkle() {
        return {{53, 50, 80},
                {53, 50, 80},
                {60, 50, 80},
                {60, 50, 80},
                {62, 50, 80},
                {62, 50, 80},
                {60, 50, 80},
                {20, 50, 80},
                {58, 50, 80},
                {58, 50, 80},
                {57, 50, 80},
                {57, 50, 80},
                {55, 50, 80},
                {55, 50, 80},
                {53, 50, 80},
                {20, 50, 80},
                {60, 50, 80},
                {60, 50, 80},
                {58, 50, 80},
                {58, 50, 80},
                {57, 50, 80},
                {57, 50, 80},
                {55, 50, 80},
                {20, 50, 80},
                {60, 50, 80},
                {60, 50, 80},
                {58, 50, 80},
                {58, 50, 80},
                {57, 50, 80},
                {57, 50, 80},
                {55, 50, 80},
                {20, 50, 80},
                {53, 50, 80},
                {53, 50, 80},
                {60, 50, 80},
                {60, 50, 80},
                {62, 50, 80},
                {62, 50, 80},
                {60, 50, 80},
                {20, 50, 80},
                {58, 50, 80},
                {58, 50, 80},
                {57, 50, 80},
                {57, 50, 80},
                {55, 50, 80},
                {55, 50, 80},
                {53, 50, 80},
                {20, 50, 80}};
    }

    std::vector<std::tuple<int,int,int>> melody_problem::one_note(size_t size) {

        std::vector<std::tuple<int,int,int>> melody;

        std::uniform_int_distribution d(get_shortest_duration(), get_longest_duration());
        std::uniform_int_distribution v(get_quietest_volume(), get_loudest_volume());
        std::uniform_int_distribution n(get_lowest_pitch(), get_highest_pitch());

        double duration = d(generator_);
        double volume   = v(generator_);
        int note        = n(generator_);

        for (size_t i = 0; i < size; ++i) {
            melody.emplace_back(std::make_tuple(note, duration, volume));
        }

        return melody;
    }

    std::vector<std::tuple<int,int,int>> melody_problem::random_problem(size_t size) {

        std::vector<std::tuple<int,int,int>> melody;

        std::uniform_int_distribution d(get_shortest_duration(), get_longest_duration());
        std::uniform_int_distribution v(get_quietest_volume(), get_loudest_volume());
        std::uniform_int_distribution n(get_lowest_pitch(), get_highest_pitch());

        for (size_t i = 0; i < size; ++i) {
            melody.emplace_back(std::make_tuple(n(generator_), d(generator_), v(generator_)));
        }

        return melody;
    }

    std::vector<std::tuple<int,int,int>> composer::melody_problem::get_melody() const {
        return this->melody_;
    }

    void melody_problem::set_melody(const std::vector<std::tuple<int,int,int>> &new_melody) {
        this->melody_ = new_melody;
    }

    std::pair<double, double> melody_problem::get_target() const {
        return this->target_;
    }

    melody_problem::problem_type melody_problem::get_type() const {
        return this->type_;
    }

    [[maybe_unused]] int melody_problem::get_rhythm() const {
        return this->rhythm_;
    }

    int melody_problem::get_lowest_pitch() const {
        return this->lowest_pitch_;
    }

    int melody_problem::get_highest_pitch() const {
        return this->highest_pitch_;
    }

    int melody_problem::get_quietest_volume() const {
        return this->quietest_volume_;
    }

    int melody_problem::get_loudest_volume() const {
        return this->loudest_volume_;
    }

    int melody_problem::get_shortest_duration() const {
        return this->shortest_duration_;
    }

    int melody_problem::get_longest_duration() const {
        return this->longest_duration_;
    }

    int melody_problem::get_slowest_tempo() const {
        return this->slowest_tempo_;
    }

    int melody_problem::get_fastest_tempo() const {
        return this->fastest_tempo_;
    }

    int melody_problem::get_pause() {
        return pause;
    }
} // namespace composer

