//
// Created by Carla on 06/03/2022.
//

#include "optimizer.h"

namespace composer {

    template <typename algorithm>
    std::default_random_engine optimizer<algorithm>::generator_ =
        std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    template <typename algorithm>
    optimizer<algorithm>::optimizer(double crossover_strength,
                                    double mutation_strength,
                                    int population_size, size_t max_iterations,
                                    const melody_problem &p)
        : crossover_strength_(crossover_strength),
          mutation_strength_(mutation_strength),
          max_iterations_(max_iterations),
          solver_(population_size, p)
    {};

    template <typename algorithm>
    void optimizer<algorithm>::select_mutation(melody &individual,
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
                break;
            default:
                individual.reverse_pulses();
                break;
            }
        }
    }

    template <typename algorithm>
    void optimizer<algorithm>::optimize() {

        melody child;
        std::uniform_real_distribution d(0.0, 1.0);

        for (int j = 0; j < this->max_iterations_; ++j) {
            for (int i = 0; i < this->solver_.get_population_size(); ++i) {

                if (d(generator_) < this->crossover_strength_) {

                    // Parents selection
                    this->solver_.select_parents();

                    // Crossover
                    child = melody::crossover(this->solver_.get_parent_1(),
                                              this->solver_.get_parent_2());

                    // Mutation
                    select_mutation(child, this->mutation_strength_);

                    // Evaluation
                    this->solver_.calculate_objective_function(child);

                    // Population
                    this->solver_.insert(child);
                }
            }

            // Parents substitution
            this->solver_.parents_substitution();
        }
    }

    template <typename algorithm> void optimizer<algorithm>::display() {
        std::cout << this->solver_;
    }
}

