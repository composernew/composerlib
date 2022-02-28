//
// Created by Carla on 29/12/2021.
//

#ifndef COMPOSER_GENETIC_ALGORITHM_H
#define COMPOSER_GENETIC_ALGORITHM_H

#include "melody.h"
#include <pareto/archive.h>

namespace composer {
    template<typename problem, typename solution>
    class genetic_algorithm {
      public:

        genetic_algorithm(double crossover_strength, double mutation_strength,
                          int population_size, size_t max_iterations, const problem &p);

        void init_population();

        void select_parents();

        static void select_mutation(solution &individual, double mutation_strength);

        static bool compare(const solution &a, const solution &b);

        void calculate_objective_function(solution &individual) const;

        void parents_substitution();

        void optimizer();

        friend std::ostream &operator<<(std::ostream &os,
                                        const genetic_algorithm &algorithm) {
            for (auto const &individual : algorithm.get_population()) {
                os << individual;
                os << '\n';
            }

            os << '\n' << algorithm.get_population()[0].get_distance() << std::endl;

            return os;
        }

        [[nodiscard]] std::pair<solution,size_t> get_best_individual() const;
        [[nodiscard]] std::vector<solution> get_best_individuals() const;
        [[nodiscard]] std::vector<solution> get_population() const;
        void set_population(const std::vector<solution> &new_population);

      private:
        int parent_1{};
        int parent_2{};
        double crossover_strength_;
        double mutation_strength_;
        int population_size_;
        size_t max_iterations_;
        problem problem_;
        std::vector<solution> population;

        std::vector<solution> best_individuals;
        std::pair<solution,size_t> best_individual;
        static std::default_random_engine generator_;
    };
} // namespace composer

#endif // COMPOSER_GENETIC_ALGORITHM_H
