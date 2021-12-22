//
// Created by Carla on 20/12/2021.
//

#ifndef COMPOSER_MELODY_PROBLEM_H
#define COMPOSER_MELODY_PROBLEM_H

#include <utility>
#include <vector>
#include <algorithm>

namespace composer {
    class melody_problem {
      public:
        explicit melody_problem();

        [[nodiscard]] std::vector<int> get_melody() const;

        static std::vector<int> c_major_double();

        static double evaluate_pitch_distribution(melody_problem const &individual);
        static double evaluate_pitch_variety(melody_problem const &individual);
        static double normalize(double value, double max, double min, double max_value, double min_value);
        static std::pair<double,double> evaluate(melody_problem const &individual);

      private:
        std::vector<int> melody_;

    };
} // namespace composer

#endif // COMPOSER_MELODY_PROBLEM_H
