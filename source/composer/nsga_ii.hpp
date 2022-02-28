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
        this->parent_1 = {};
        this->parent_2 = {};

        init_population();
    }

    template <typename problem, typename solution>
    void nsga_ii<problem, solution>::init_population() {

        for (size_t i = 0; i < this->population_size_; ++i) {

            if (this->problem_.get_type() == problem::problem_type::random) {
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

    template <typename problem, typename solution>
    void nsga_ii<problem, solution>::parents_substitution() {

        auto iter = this->ranking_.rbegin_front();

        while ((this->ranking_.size() - iter->size()) >= this->population_size_) {
            size_t iter_size = iter->size();
            for (size_t i = 0; i < iter_size; ++i) {
                this->ranking_.erase(iter->begin()->first);
            }
        }

        if (this->ranking_.size() != this->population_size_) {
            using pareto = pareto::front<double, 2, solution>;
            std::vector<typename pareto::const_iterator> distances;

            size_t selected_individuals = this->population_size_ - (this->ranking_.size() - iter->size());

            for (auto it = iter->begin(); it != iter->end(); ++it) {
                distances.emplace_back(it);
            }

            std::partial_sort(distances.begin(),
                              distances.begin()+(selected_individuals),
                              distances.end(),
                              [this](const auto &a, const auto &b) {
                                  return this->ranking_.crowding_distance(b->first) <
                                         this->ranking_.crowding_distance(a->first);
                              });

            for (size_t i = selected_individuals; i < distances.size(); ++i) {
                this->ranking_.erase(distances[i]->first);
            }
        }
    }

}// namespace composer
