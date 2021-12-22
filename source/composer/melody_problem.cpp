//
// Created by Carla on 21/12/2021.
//

#include "melody_problem.h"

namespace composer {

    melody_problem::melody_problem() {
        this->melody_ = c_major_double();
    }

    std::vector<int> melody_problem::c_major_double() {
        return {60, 62, 64, 65, 67, 69, 71, 72, 60, 62, 64, 65, 67, 69, 71, 72};
    }

    std::vector<int> composer::melody_problem::get_melody() const {
        return this->melody_;
    }

} // namespace composer

