//
// Created by Carla on 02/12/2021.
//

#include <iostream>
#include <vector>
#include "composer/melody_problem.h"
#include "composer/genetic_algorithm.h"

using namespace composer;

/*void save_results(auto &results, size_t population_size, size_t individual_size,
                  double mutation_strength, int max_iterations) {

    int num_executions = 100;
    double best_solution = 2;
    int best_execution = 0;
    std::ofstream file("teste2.txt");

    /*for (int i = 0; i < num_executions; ++i) {

        file << "Execution: " << i << " - Best solution: " << std::get<1>(results[i])
             << " - Best stime: " << std::get<2>(results[i]);

        file << " - Easy time: " << std::get<3>(results[i]) << " - Medium time: "
            << std::get<4>(results[i]) << " - Hard time: " << std::get<5>(results[i]) << "\n";

        if (std::get<1>(results[i]) < best_solution) {
            best_solution = std::get<1>(results[i]);
            best_execution = i;
        }
    }

    // Best execution results
    // Save results
    file << "POPULATION SIZE: " << population_size << "\n";
    file << "INDIVIDUAL SIZE: " << individual_size << "\n";
    file << "MAX ITERATIONS: " << max_iterations << "\n";
    file << "MUTATION STRENGTH: " << mutation_strength << "\n";
    file << "PROBLEM TYPE: MAX MIN \n\n";

    file << "FINAL MELODIES\n";

    for (const melody &melody_ : std::get<0>(results[best_execution])) {
        file << "[";
        for (const int &note : melody_.get_melody()) {
            file << note << " ";
        }
        file << "]\n";
    }

    file << "\nPOPULATION DISTANCE\n";

    for (const melody &melody_ : std::get<0>(results[best_execution])) {
        file << "[" << melody_.get_distance() << "]\n";
    }

    file << "\nPOPULATION VALUES\n";

    for (const melody &melody_ : std::get<0>(results[best_execution])) {
        file << "(" << melody_.get_valence_arousal().first << "," <<
            melody_.get_valence_arousal().second << ")\n";
    }

    file.close();
}*/

int main () {

    int population_size = 500;
    double mutation_strength = 0.1;
    double crossover_strength = 0.5;
    int max_iterations = 1000;

    const melody_problem problem({0.5, 0.5}, melody_problem::problem_type::c_major_double);

    composer::genetic_algorithm ga(crossover_strength, mutation_strength, population_size, max_iterations, problem);

    ga.optimizer();

    ga.display();

    return 0;
}