//
// Created by Carla on 17/02/2022.
//

#include "nsga_ii.h"

namespace composer {

    template <typename problem, typename solution>
    nsga_ii<problem, solution>::nsga_ii(double crossover_strength,
                                        double mutation_strength,
                                        int population_size,
                                        size_t max_iterations,
                                        const problem &p)
        : crossover_strength_(crossover_strength),
          mutation_strength_(mutation_strength),
          population_size_(population_size),
          max_iterations_(max_iterations),
          problem_(p)
    {
        this->ranking_ = pareto::archive<double, 2, solution>((2*this->population_size_), {pareto::min, pareto::min}, this->allocator_);
    }

    template <typename problem, typename solution>
    void nsga_ii<problem, solution>::init_population() {

    }

}// namespace composer
