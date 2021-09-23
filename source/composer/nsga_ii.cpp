//
// Created by Carla on 24/06/2021.
//

#include <iostream>
#include <composer/melody.h>
#include <pareto/archive.h>
#include "MidiFile.h"
#include "Options.h"
#include "nsga_ii.h"

namespace composer {

    void nsga_ii() {

        static std::default_random_engine generator_;
        generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

        // Problem parameters
        const size_t problem_size = 1000;
        const size_t individual_size = 8;

        // Create problem
        composer::melody problem(problem_size, individual_size);
        problem.display();

        // Search parameters
        const size_t max_iterations = 100;

        std::vector<int> candidate_solution(4);
        std::vector<std::tuple<double,double>> fitness;
        std::tuple<double, double> evaluation;
        size_t capacity = 100;
        const std::pmr::polymorphic_allocator<std::pair<const pareto::point<double, 2, unsigned >, unsigned int>> allocator;
        pareto::archive<double, 2, double> pareto_archive(capacity, {pareto::min, pareto::min}, allocator);
        std::vector<std::vector<int>> solution;
        std::vector<std::tuple<double, int>> crowding_distance;
        double valence, arousal;

        for (size_t i = 0; i < problem_size; ++i) {
            evaluation = problem.evaluate(problem.get_melody()[i]);
            valence = std::get<0>(evaluation);
            arousal = std::get<1>(evaluation);
            pareto_archive(valence, arousal) = i;
        }

        for (size_t i = 0; i < max_iterations; ++i) {
            std::cout << "Solution #" << i + 1 << std::endl;

            for (size_t j = 0; j < problem_size; ++j) {

                // Selection and crossover
                std::uniform_int_distribution<int> d(0, (problem_size-1));
                candidate_solution = problem.crossover(problem.get_melody()[d(generator_)]);

                // Mutation
                problem.reverse_pulses(candidate_solution, 0.0005);

                problem.add_individual(candidate_solution);

                // Evaluation
                evaluation = problem.evaluate(candidate_solution);
                valence = std::get<0>(evaluation);
                arousal = std::get<1>(evaluation);

                // No dominated ranking
                pareto_archive(valence, arousal) = (j+problem_size);
            }

            auto it = pareto_archive.begin_front();

            if (pareto_archive.size() > problem_size) {
                for (const auto &[k, v] : pareto_archive) {
                    crowding_distance.push_back({pareto_archive.crowding_distance(k),v});
                }

                std::sort(crowding_distance.begin(), crowding_distance.end());

                for (size_t i = 0; i < problem_size; ++i) {
                    int position = std::get<1>(crowding_distance[i]);
                    solution.push_back(problem.get_melody()[position]);
                }
            }
            else if (pareto_archive.size() < problem_size) {
                for (it = pareto_archive.begin_front(); (it != pareto_archive.end_front() || solution.size() == problem_size); ++it) {
                    for (const auto &[k, v] : pareto_archive) {
                        solution.push_back(problem.get_melody()[v]);
                    }

                }
            }
            else {
                for (const auto &[k, v] : pareto_archive) {
                    solution.push_back(problem.get_melody()[v]);
                }
            }

            problem.clear();

            for (std::vector<int> individual : solution) {
                problem.add_individual(individual);
            }

            solution.clear();
        }

        std::cout << "Solution:" << std::endl;
        problem.display();
    }
}

