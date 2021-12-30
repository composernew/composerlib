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
        init_population();
        this->parent_1_ = 0;
        this->parent_2_ = 0;
    }

    void genetic_algorithm::init_population() {

        for (size_t i = 0; i < this->population_size_; ++i) {
            melody individual(this->problem_);
            this->population_.emplace_back(individual);
        }
    }

    void genetic_algorithm::init_random_population() {

        melody_problem problem(this->problem_.get_target(), melody_problem::problem_type::random,
                               this->problem_.get_melody().size());

        for (size_t i = 0; i < this->population_size_; ++i) {
            melody individual(problem);
            this->population_.emplace_back(individual);
        }
    }

    void genetic_algorithm::select_parents() {

        std::uniform_int_distribution d(0, (this->population_size_-1));
        this->parent_1_ = d(generator_);
        this->parent_2_ = d(generator_);
    }

    void genetic_algorithm::select_mutation(melody &individual,
                                            double mutation_strength) {

        std::uniform_real_distribution real_d(0.0, 1.0);

        if (real_d(generator_) < mutation_strength) {
            std::uniform_int_distribution d(1, 4);

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
            default:
                individual.reverse_pulses();
                break;
            }
        }
    }

    bool genetic_algorithm::compare(const melody &a, const melody &b) {
        return a.get_distance() < b.get_distance();
    }

    void genetic_algorithm::calculate_objective_function(melody &individual) {
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
                    child = melody::crossover(this->population_[this->parent_1_],
                                              this->population_[this->parent_2_]);

                    // Mutation
                    select_mutation(child, this->mutation_strength_);

                    // Evaluation
                    child.evaluate();
                    calculate_objective_function(child);

                    // Population
                    this->population_.emplace_back(child);
                }
            }

            // Parents substitution
            std::sort(this->population_.begin(), this->population_.end(), compare);
            this->population_.erase(this->population_.begin()+ this->population_size_,
                             this->population_.end());
        }
    }

    void genetic_algorithm::display() const {

        for (auto const &individual : this->population_) {
            for (auto const &item : individual.get_melody()) {
                std::cout << item << ' ';
            }
            std::cout << std::endl;
        }

        std::cout << std::endl << population_[0].get_distance() << std::endl;
    }
} // namespace composer
