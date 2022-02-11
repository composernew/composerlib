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

      private:

        double crossover_strength_;
        double mutation_strength_;
        int population_size_;
        size_t max_iterations_;
        problem problem_;

        const std::pmr::polymorphic_allocator<std::pair<const pareto::point<double, 2, solution>, solution>> allocator_;
        pareto::archive<double, 2, solution> ranking_;
    };
}// namespace composer



#endif // COMPOSER_NSGA_II_H
