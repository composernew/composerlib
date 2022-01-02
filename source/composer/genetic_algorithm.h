//
// Created by Carla on 29/12/2021.
//

#ifndef COMPOSER_GENETIC_ALGORITHM_H
#define COMPOSER_GENETIC_ALGORITHM_H

#include "melody.h"

namespace composer {
    class genetic_algorithm {
      public:

        genetic_algorithm(double crossover_strength, double mutation_strength,
                          int population_size, size_t max_iterations, melody_problem problem);

        void init_population();
        void init_random_population();

        void select_parents();

        static void select_mutation(melody &individual, double mutation_strength);

        static bool compare(const melody &a, const melody &b);

        void calculate_objective_function(melody &individual) const;

        void optimizer();

        void display() const;

        [[nodiscard]] std::pair<melody,size_t> get_best_individual() const;
        [[nodiscard]] std::vector<melody> get_half_evolution() const;
        [[nodiscard]] std::vector<melody> get_best_individuals() const;
        [[nodiscard]] std::vector<melody> get_population() const;

      private:
        int parent_1;
        int parent_2;
        double crossover_strength_;
        double mutation_strength_;
        int population_size_;
        size_t max_iterations_;
        melody_problem problem_;
        std::vector<melody> population;

        std::vector<melody> best_individuals;
        std::vector<melody> half_evolution;
        std::pair<melody,size_t> best_individual;
        static std::default_random_engine generator_;
    };
} // namespace composer

#endif // COMPOSER_GENETIC_ALGORITHM_H
