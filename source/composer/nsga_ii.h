//
// Created by Carla on 17/02/2022.
//

#ifndef COMPOSER_NSGA_II_H
#define COMPOSER_NSGA_II_H

#include <pareto/archive.h>

namespace composer {

    template<typename problem, typename solution>
    class nsga_ii {

      public:
        nsga_ii(double crossover_strength, double mutation_strength,
                int population_size, size_t max_iterations, const problem &p);

        void init_population();

        void select_parents();

        void calculate_objective_function(solution &individual) const;

      private:
        double crossover_strength_;
        double mutation_strength_;
        int population_size_;
        size_t max_iterations_;
        problem problem_;
        typename pareto::archive<double, 2, solution>::iterator parent_1;
        typename pareto::archive<double, 2, solution>::iterator parent_2{};

        pareto::archive<double, 2, solution> ranking_;
        static std::default_random_engine generator_;
    };
}// namespace composer

#endif // COMPOSER_NSGA_II_H
