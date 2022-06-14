//
// Created by Carla on 20/12/2021.
//

#ifndef COMPOSER_MELODY_PROBLEM_H
#define COMPOSER_MELODY_PROBLEM_H

#include <utility>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

namespace composer {
    class melody_problem {
      public:

        enum class problem_type {c_major_double, twinkle, random, one_note};

        melody_problem(std::pair<double,double> target, problem_type type, size_t size = 16)
            : target_(std::move(target)),
              type_(type) {

            switch (type) {

                case problem_type::c_major_double:
                    this->melody_ = c_major_double();
                    break;

                case problem_type::twinkle:
                    this->melody_ = twinkle();
                    break;

                case problem_type::random:
                    this->melody_ = random_problem(size);
                    break;

                default:
                    this->melody_ = one_note(size);
                    break;
            }
        };

        [[nodiscard]] std::vector<int> get_melody() const;
        void set_melody(std::vector<int> const &new_melody);

        static std::vector<int> c_major_double();
        static std::vector<int> twinkle();
        static std::vector<int> random_problem(size_t size);
        static std::vector<int> one_note(size_t size);

        static double evaluate_pitch_distribution(melody_problem const &individual);
        static double evaluate_pitch_variety(melody_problem const &individual);
        static double normalize(double value, double max, double min, double max_value, double min_value);
        static std::pair<double,double> evaluate(melody_problem const &individual);

        [[nodiscard]] std::pair<double,double> get_target() const;
        [[nodiscard]] problem_type get_type() const;

      private:
        std::vector<int> melody_;
        std::pair<double,double> target_;
        problem_type type_;

        static std::default_random_engine generator_;
    };
} // namespace composer

#endif // COMPOSER_MELODY_PROBLEM_H
