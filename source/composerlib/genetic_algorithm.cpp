//
// Created by Carla on 29/12/2021.
//

#include "genetic_algorithm.h"
#include <utility>

namespace composer {

    std::default_random_engine genetic_algorithm::generator_ =
        std::default_random_engine(
            std::chrono::system_clock::now().time_since_epoch().count());

    [[maybe_unused]] genetic_algorithm::genetic_algorithm(int population_size,
                                                          melody_problem p,
                                                          double diversity_strength)
        : parent_1(0), parent_2(0), population_size_(population_size),
          problem_(std::move(p)), diversity_strength_(diversity_strength) {
        init_population();
    }

    void genetic_algorithm::insert(const melody &individual) {
        this->population.emplace_back(individual);
    }

    void genetic_algorithm::init_population() {

        for (size_t i = 0; i < this->population_size_; ++i) {

            std::uniform_real_distribution d(0.0, 1.0);

            melody individual(this->problem_);

            if (d(generator_) < this->diversity_strength_) {
                individual.set_melody(this->problem_.random_problem(
                    this->problem_.get_melody().size()));
            }

            insert(individual);
        }
    }

    void genetic_algorithm::select_parents() {

        std::uniform_int_distribution d(0, (this->population_size_ - 1));
        this->parent_1 = d(generator_);
        this->parent_2 = d(generator_);
    }

    bool genetic_algorithm::compare(const melody &a, const melody &b) {
        return a.get_distance() < b.get_distance();
    }

    void
    genetic_algorithm::calculate_objective_function(melody &individual) const {

        individual.evaluate();

        individual.set_distance(melody::euclidean_distance(
            this->problem_.get_target(), individual.get_valence_arousal()));
    }

    void genetic_algorithm::parents_substitution() {

        std::sort(this->population.begin(), this->population.end(), compare);
        this->population.erase(this->population.begin() +
                                   this->population_size_,
                               this->population.end());
    }

    std::vector<melody> genetic_algorithm::get_population() const {
        return this->population;
    }

    [[maybe_unused]] void genetic_algorithm::set_population(
        const std::vector<melody> &new_population) {

        this->population = new_population;
    }

    size_t genetic_algorithm::get_population_size() const {
        return this->population_size_;
    }

    melody genetic_algorithm::get_parent_1() {
        return this->population[this->parent_1];
    }

    melody genetic_algorithm::get_parent_2() {
        return this->population[this->parent_2];
    }
} // namespace composer
