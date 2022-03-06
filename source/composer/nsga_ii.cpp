//
// Created by Carla on 17/02/2022.
//

#include "nsga_ii.h"

namespace composer {

    using front_it = typename pareto::front<double, 2, melody>::iterator;

    std::default_random_engine nsga_ii::generator_ =
        std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    nsga_ii::nsga_ii(int population_size, const melody_problem &p)
        : population_size_(population_size),
          problem_(p)
    {
        this->ranking_ = pareto::archive<double, 2, melody>((2 * this->population_size_), {pareto::min, pareto::min});

        this->parent_1 = {};
        this->parent_2 = {};

        init_population();
    }

    void nsga_ii::insert(const melody &individual) {

        this->ranking_.insert(std::make_pair(
            typename pareto::archive<double, 2, melody>::key_type(
                {individual.get_valence_arousal().first,
                 individual.get_valence_arousal().second}),
            individual)
        );
    }

    void nsga_ii::init_population() {

        for (size_t i = 0; i < this->population_size_; ++i) {

            /*if (this->problem_.get_type() == melody_problem::problem_type::random) {
                this->problem_.set_melody(melody_problem::random_problem(this->problem_.get_melody().size()));
            }*/

            melody individual(this->problem_);

            insert(individual);
        }
    }

    void nsga_ii::select_parents() {

        using archive_key_type = typename pareto::archive<double, 2, melody>::key_type;
        std::uniform_real_distribution<double> d(0, 1);

        auto key = archive_key_type({d(generator_), d(generator_)});
        this->parent_1 = this->ranking_.find_nearest(key);

        key = archive_key_type({d(generator_), d(generator_)});
        this->parent_2 = this->ranking_.find_nearest(key);
    }

    void nsga_ii::calculate_objective_function(melody &individual) const {
        individual.evaluate();
    }

    void nsga_ii::parents_substitution() {

        auto iter = this->ranking_.rbegin_front();
        std::vector<std::pair<const pareto::point<double, 2>, melody>> new_population;
        std::vector<std::pair<pareto::point<double, 2>, melody>> distances;

        while ((iter->size() + new_population.size()) <= this->population_size_) {
            for (const auto &element : *iter) {
                new_population.emplace_back(element);
            }
            ++iter;
        }

        if (this->ranking_.size() != this->population_size_) {

            size_t selected_individuals = this->population_size_ - (this->ranking_.size() - iter->size());

            for (const auto &element : *iter) {
                distances.emplace_back(element);
            }

            std::partial_sort(distances.begin(),
                              distances.begin()+(selected_individuals),
                              distances.end(),
                              [this](const auto &a, const auto &b) {
                                  return this->ranking_.crowding_distance(b.first) <
                                         this->ranking_.crowding_distance(a.first);
                              });

            distances.erase(distances.begin()+selected_individuals, distances.end());
        }

        this->ranking_.clear();
        this->ranking_.insert(new_population.begin(), new_population.end());
        this->ranking_.insert(distances.begin(), distances.end());
    }

    size_t nsga_ii::get_population_size() {
        return this->population_size_;
    }

    melody nsga_ii::get_parent_1() {
        return this->parent_1->second;
    }

    melody nsga_ii::get_parent_2() {
        return this->parent_2->second;
    }
} // namespace composer
