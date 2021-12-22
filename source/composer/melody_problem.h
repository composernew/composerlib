//
// Created by Carla on 20/12/2021.
//

#ifndef COMPOSER_MELODY_PROBLEM_H
#define COMPOSER_MELODY_PROBLEM_H

#include <utility>
#include <vector>

namespace composer {
    class melody_problem {
      public:
        explicit melody_problem();

        [[nodiscard]] std::vector<int> get_melody() const;

        static std::vector<int> melody_problem::c_major_double();

      private:
        std::vector<int> melody_;

    };
} // namespace composer

#endif // COMPOSER_MELODY_PROBLEM_H
