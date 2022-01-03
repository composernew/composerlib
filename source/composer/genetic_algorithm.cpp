//
// Created by Carla on 29/12/2021.
//

#include "genetic_algorithm.h"
#include <utility>

namespace composer {

    std::default_random_engine genetic_algorithm::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    genetic_algorithm::genetic_algorithm(double crossover_strength,
                                         double mutation_strength,
                                         int population_size,
                                         size_t max_iterations,
                                         melody_problem problem)

        : crossover_strength_(crossover_strength),
          mutation_strength_(mutation_strength),
          population_size_(population_size),
          max_iterations_(max_iterations),
          problem_(std::move(problem))
    {
        if (this->problem_.get_type() == melody_problem::problem_type::random)
            init_random_population();
        else init_population();

        this->parent_1 = 0;
        this->parent_2 = 0;

        this-> best_individual = {melody(problem_), 0};
        this->best_individuals = {};
    }

    void genetic_algorithm::init_population() {

        for (size_t i = 0; i < this->population_size_; ++i) {
            melody individual(this->problem_);
            this->population.emplace_back(individual);
        }
    }

    void genetic_algorithm::init_random_population() {

        for (size_t i = 0; i < this->population_size_; ++i) {
            melody_problem problem(this->problem_.get_target(), melody_problem::problem_type::random,
                                   this->problem_.get_melody().size());
            melody individual(problem);
            this->population.emplace_back(individual);
        }
    }

    void genetic_algorithm::select_parents() {

        std::uniform_int_distribution d(0, (this->population_size_-1));
        this->parent_1 = d(generator_);
        this->parent_2 = d(generator_);
    }

    void genetic_algorithm::select_mutation(melody &individual,
                                            double mutation_strength) {

        std::uniform_real_distribution real_d(0.0, 1.0);

        if (real_d(generator_) < mutation_strength) {
            std::uniform_int_distribution d(1, 3);

            int mutation = d(generator_);

            switch (mutation) {
            case 1:
                individual.simple_mutation();
                break;
            case 2:
                individual.reverse_measure();
                break;
            case 3:
                individual.exchange_pulses();
                break;
            default:
                break;
            }
        }
    }

    bool genetic_algorithm::compare(const melody &a, const melody &b) {
        return a.get_distance() < b.get_distance();
    }

    void genetic_algorithm::calculate_objective_function(melody &individual) const {
        individual.set_distance(melody::euclidean_distance(this->problem_.get_target(),
                                                           individual.get_valence_arousal()));
    }

    void genetic_algorithm::optimizer() {

        melody child;
        std::uniform_real_distribution d(0.0, 1.0);

        for (int j = 0; j < this->max_iterations_; ++j) {
            for (int i = 0; i < this->population_size_; ++i) {

                if (d(generator_) < this->crossover_strength_) {

                    // Parents selection
                    select_parents();

                    // Crossover
                    child = melody::crossover(this->population[this->parent_1],
                                              this->population[this->parent_2]);

                    // Mutation
                    select_mutation(child, this->mutation_strength_);

                    // Evaluation
                    child.evaluate();
                    calculate_objective_function(child);

                    // Population
                    this->population.emplace_back(child);
                }
            }

            // Parents substitution
            std::sort(this->population.begin(), this->population.end(), compare);
            this->population.erase(this->population.begin()+ this->population_size_,
                             this->population.end());

            if (compare(this->population[0], this->best_individual.first)) {
                this->best_individual = {population[0], j};
            }

            best_individuals.emplace_back(population[0]);

            if (j == (this->max_iterations_/2)) {
                this->half_evolution = this->population;
            }
        }
    }

    void genetic_algorithm::display() const {

        for (auto const &individual : this->population) {
            for (auto const &item : individual.get_melody()) {
                std::cout << item << ' ';
            }
            std::cout << std::endl;
        }

        std::cout << std::endl << population[0].get_distance() << std::endl;
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

    std::vector<melody> genetic_algorithm::get_half_evolution() const {
        return this->half_evolution;
    }
} // namespace composer
