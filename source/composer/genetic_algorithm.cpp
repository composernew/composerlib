//
// Created by Carla on 29/12/2021.
//

#include "genetic_algorithm.h"

namespace composer {

    std::default_random_engine genetic_algorithm::generator_ =
        std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    genetic_algorithm::genetic_algorithm(int population_size,
                                         const melody_problem &p)
        : population_size_(population_size),
          problem_(p)
    {
        init_population();

        this->parent_1 = 0;
        this->parent_2 = 0;

        this->best_individual = {melody(this->problem_), 0};
        this->best_individuals = {};
    }

    void genetic_algorithm::insert(const melody &individual) {
        this->population.emplace_back(individual);
    }

    void genetic_algorithm::init_population() {

        for (size_t i = 0; i < this->population_size_; ++i) {

            if(this->problem_.get_type() == melody_problem::problem_type::random) {
                this->problem_.set_melody(melody_problem::random_problem(this->problem_.get_melody().size()));
            }

            melody individual(this->problem_);
            this->population.emplace_back(individual);
        }
    }

    void genetic_algorithm::select_parents() {

        std::uniform_int_distribution d(0, (this->population_size_-1));
        this->parent_1 = d(generator_);
        this->parent_2 = d(generator_);
    }

    bool genetic_algorithm::compare(const melody &a, const melody &b) {
        return a.get_distance() < b.get_distance();
    }

    void genetic_algorithm::calculate_objective_function(melody &individual) const {

        individual.evaluate();

        individual.set_distance(melody::euclidean_distance(this->problem_.get_target(),
                                                           individual.get_valence_arousal()));
    }

    void genetic_algorithm::parents_substitution() {

        std::sort(this->population.begin(), this->population.end(), compare);
        this->population.erase(this->population.begin()+ this->population_size_,
                               this->population.end());
    }

    std::pair<melody, size_t> genetic_algorithm::get_best_individual() const {
        return this->best_individual;
    }

    std::vector<melody> genetic_algorithm::get_best_individuals() const {
        return this->best_individuals;
    }

    std::vector<melody> genetic_algorithm::get_population() const {
        return this->population;
    }

    void genetic_algorithm::set_population(
        const std::vector<melody> &new_population) {

        this->population = new_population;
    }

    size_t genetic_algorithm::get_population_size() {
        return this->population_size_;
    }

    melody genetic_algorithm::get_parent_1() {
        return this->population[this->parent_1];
    }

    melody genetic_algorithm::get_parent_2() {
        return this->population[this->parent_2];
    }
} // namespace composer
