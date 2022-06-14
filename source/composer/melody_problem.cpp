//
// Created by Carla on 21/12/2021.
//

#include "melody_problem.h"
#include <utility>

namespace composer {

    std::default_random_engine melody_problem::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    std::vector<int> melody_problem::c_major_double() {
        return {60, 62, 64, 65, 67, 69, 71, 72, 60, 62, 64, 65, 67, 69, 71, 72};
    }

    std::vector<int> melody_problem::twinkle() {
        return {53, 53, 60, 60, 62, 62, 60, 20, 58, 58, 57, 57, 55, 55, 53, 20,
                60, 60, 58, 58, 57, 57, 55, 20, 60, 60, 58, 58, 57, 57, 55, 20,
                53, 53, 60, 60, 62, 62, 60, 20, 58, 58, 57, 57, 55, 55, 53, 20};
    }

    std::vector<int> melody_problem::one_note(size_t size) {
        std::vector<int> melody;
        std::uniform_int_distribution d(20, 108);
        int note = d(generator_);

        for (size_t i = 0; i < size; ++i) {
            melody.emplace_back(note);
        }

        return melody;
    }

    std::vector<int> melody_problem::random_problem(size_t size) {

        std::vector<int> melody;
        std::uniform_int_distribution d(20, 108);

        for (size_t i = 0; i < size; ++i) {
            melody.emplace_back(d(generator_));
        }

        return melody;
    }

    std::vector<int> composer::melody_problem::get_melody() const {
        return this->melody_;
    }

    void melody_problem::set_melody(const std::vector<int> &new_melody) {
        this->melody_ = new_melody;
    }

    std::pair<double, double> melody_problem::get_target() const {
        return this->target_;
    }

    melody_problem::problem_type melody_problem::get_type() const {
        return this->type_;
    }

} // namespace composer

