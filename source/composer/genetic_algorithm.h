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

        void calculate_objective_function(melody &individual);

        void optimizer();

        void display() const;

      private:
        int parent_1_;
        int parent_2_;
        double crossover_strength_;
        double mutation_strength_;
        int population_size_;
        size_t max_iterations_;
        std::vector<melody> population_;
        melody_problem problem_;

        static std::default_random_engine generator_;
    };
} // namespace composer

#endif // COMPOSER_GENETIC_ALGORITHM_H
