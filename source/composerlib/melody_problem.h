//
// Created by Carla on 20/12/2021.
//

#ifndef COMPOSER_MELODY_PROBLEM_H
#define COMPOSER_MELODY_PROBLEM_H

#include <algorithm>
#include <chrono>
#include <random>
#include <utility>
#include <vector>

namespace composer {
    class melody_problem {
      public:
        enum class problem_type { c_major_double, twinkle, random, one_note };
        enum class feature_type {
            pause = 20,
            highest_pitch = 108,
            lowest_volume = 0,
            highest_volume = 127,
            lowest_duration = 0,
            highest_duration = 100,
            slowest_tempo = 40,
            fastest_tempo = 208
        };

        melody_problem(problem_type type, int rhythm = 120,
                       std::pair<double, double> target = {0., 0.},
                       size_t size = 16)
            : target_(std::move(target)), type_(type), rhythm_(rhythm) {

            std::uniform_int_distribution d(
                static_cast<int>(feature_type::slowest_tempo),
                static_cast<int>(feature_type::fastest_tempo));

            switch (type) {

                case problem_type::c_major_double:
                    this->melody_ = c_major_double();
                    break;

                case problem_type::twinkle:
                    this->melody_ = twinkle();
                    break;

                case problem_type::random:
                    this->melody_ = random_problem(size);
                    this->rhythm_ = d(generator_);
                    break;

                case problem_type::one_note:
                    this->melody_ = one_note(size);
                    break;

                default:
                    break;
            }
        };

        [[nodiscard]] std::vector<std::tuple<int,int,int>> get_melody() const;
        void set_melody(std::vector<std::tuple<int,int,int>> const &new_melody);

        [[maybe_unused]] [[nodiscard]] int get_rhythm() const;
        [[maybe_unused]] void set_rhythm(const int &new_rhythm);

        static std::vector<std::tuple<int,int,int>> c_major_double();
        static std::vector<std::tuple<int,int,int>> twinkle();
        static std::vector<std::tuple<int,int,int>> random_problem(size_t size);
        static std::vector<std::tuple<int,int,int>> one_note(size_t size);

        [[nodiscard]] std::pair<double,double> get_target() const;
        [[maybe_unused]] void set_target(std::pair<double,double> &new_target);

        [[nodiscard]] problem_type get_type() const;

      private:
        std::vector<std::tuple<int,int,int>> melody_;
        int rhythm_;
        std::pair<double,double> target_;
        problem_type type_;

        static std::default_random_engine generator_;
    };
} // namespace composer

#endif // COMPOSER_MELODY_PROBLEM_H
