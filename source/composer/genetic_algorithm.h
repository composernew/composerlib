//
// Created by Carla on 29/12/2021.
//

#ifndef COMPOSER_GENETIC_ALGORITHM_H
#define COMPOSER_GENETIC_ALGORITHM_H

#include "melody.h"
#include <utility>
#include <pareto/archive.h>

namespace composer {

    class genetic_algorithm {
      public:

        genetic_algorithm(int population_size, melody_problem p);

        void insert(const melody &individual);

        void init_population();

        void select_parents();

        static bool compare(const melody &a, const melody &b);

        void calculate_objective_function(melody &individual) const;

        void parents_substitution();

        [[nodiscard]] size_t get_population_size() const;

        melody get_parent_1();
        melody get_parent_2();

        [[nodiscard]] std::vector<melody> get_population() const;
        void set_population(const std::vector<melody> &new_population);

        melody_problem get_problem();

        friend std::ostream &operator<<(std::ostream &os,
                                        const genetic_algorithm &algorithm) {
            for (auto const &individual : algorithm.get_population()) {
                os << individual;
                os << '\n';
            }

            os << '\n' << algorithm.get_population()[0].get_distance() << std::endl;

            return os;
        }

      private:
        int parent_1{};
        int parent_2{};
        int population_size_;
        melody_problem problem_;
        std::vector<melody> population;

        static std::default_random_engine generator_;
    };
} // namespace composer

#endif // COMPOSER_GENETIC_ALGORITHM_H
