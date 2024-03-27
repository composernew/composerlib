//
// Created by Carla on 17/02/2022.
//

#ifndef COMPOSER_NSGA_II_H
#define COMPOSER_NSGA_II_H

#include "melody_problem.h"
#include "melody.h"
#include <pareto/archive.h>

namespace composer {

    class nsga_ii {

      public:

        nsga_ii(int population_size, melody_problem p);

        void insert(const melody &individual);

        void init_population();

        void select_parents();

        static void calculate_objective_function(melody &individual);

        void parents_substitution();

        [[nodiscard]] size_t get_population_size() const;

        [[nodiscard]] melody get_parent_1() const;
        [[nodiscard]] melody get_parent_2() const;

        pareto::archive<double, 4, melody> get_population();

        friend std::ostream &operator<<(std::ostream &os,
                                        const nsga_ii &algorithm) {

            for (auto it = algorithm.ranking_.begin_front(); it != algorithm.ranking_.end_front(); ++it) {
                for (const auto &[k, v] : *it) {
                    os << k << " -> " << v << std::endl;
                }
            }

            return os;
        }

      private:

        using archive_key_type = typename pareto::archive<double, 4, melody>::key_type;

        std::pair<archive_key_type, melody> parent_1;
        std::pair<archive_key_type, melody> parent_2;
        int population_size_;
        melody_problem problem_;
        pareto::archive<double, 4, melody> ranking_;
    };
}// namespace composer

#endif // COMPOSER_NSGA_II_H
