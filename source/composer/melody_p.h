//
// Created by Carla on 01/08/2021.
//

#include <iostream>
#include <vector>
#include <random>
#include <chrono>

#ifndef COMPOSER_MELODY_P_H
#define COMPOSER_MELODY_P_H

namespace composer {

    class melody_p {
      public:
        explicit melody_p(const size_t &problem_size, const size_t &individual_size);
      private:
        std::vector<std::vector<int>> melody_;
        static std::default_random_engine generator_;
    };
}

#endif // COMPOSER_MELODY_P_H
