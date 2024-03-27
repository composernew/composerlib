//
// Created by Carla on 06/03/2022.
//

#ifndef COMPOSER_OPTIMIZER_H
#define COMPOSER_OPTIMIZER_H

#include "melody.h"

namespace composer {

    template<typename algorithm>
    class optimizer {

      public:

        optimizer(double crossover_strength, double mutation_strength,
                  int population_size, size_t max_iterations,
                  const melody_problem &p);

        static void select_mutation(melody &individual, double mutation_strength);

        void optimize();

        void display();

      private:

        size_t max_iterations_;
        double crossover_strength_;
        double mutation_strength_;
        algorithm solver_;

        static std::default_random_engine generator_;
    };
} // namespace composer

#endif // COMPOSER_OPTIMIZER_H
