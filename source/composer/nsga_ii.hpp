//
// Created by Carla on 17/02/2022.
//

#include "nsga_ii.h"

namespace composer {

    template<typename problem, typename solution>
    using front_it = typename pareto::front<double, 2, solution>::iterator;

    template<typename problem, typename solution>
    std::default_random_engine nsga_ii<problem, solution>::generator_ =
        std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

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
        this->ranking_ = pareto::archive<double, 2, solution>((2 * this->population_size_), {pareto::min, pareto::min});
        this->parent_1 = this->ranking_.begin();
        this->parent_2 = this->ranking_.begin();

        init_population();
    }

    template <typename problem, typename solution>
    void nsga_ii<problem, solution>::init_population() {

        for (size_t i = 0; i < this->population_size_; ++i) {

            if(this->problem_.get_type() == problem::problem_type::random) {
                this->problem_.set_melody(problem::random_problem(this->problem_.get_melody().size()));
            }

            solution individual(this->problem_);

            this->ranking_.insert(std::make_pair(
                typename pareto::archive<double, 2, solution>::key_type(
                    {individual.get_valence_arousal().first,
                     individual.get_valence_arousal().second}),
                individual)
            );
        }
    }

    template <typename problem, typename solution>
    void nsga_ii<problem, solution>::select_parents() {

        using archive_key_type = typename pareto::archive<double, 2, solution>::key_type;
        std::uniform_real_distribution<double> d(0, 1);

        auto key = archive_key_type({d(generator_), d(generator_)});
        this->parent_1 = this->ranking_.find_nearest(key);

        key = archive_key_type({d(generator_), d(generator_)});
        this->parent_2 = this->ranking_.find_nearest(key);
    }

    template <typename problem, typename solution>
    void nsga_ii<problem, solution>::calculate_objective_function(
        solution &individual) const {

        individual.evaluate();
    }

}// namespace composer
