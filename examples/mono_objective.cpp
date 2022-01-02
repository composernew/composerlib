//
// Created by Carla on 02/12/2021.
//

#include <iostream>
#include <fstream>
#include <vector>
#include "composer/melody_problem.h"
#include "composer/genetic_algorithm.h"

using namespace composer;

void save_melody(std::ofstream &file, std::vector<int> const &melody) {
    for (auto const &note : melody) {
        file << note << ' ';
    }
    file << '\n';
}

void save_population(std::string const &filename, std::vector<melody> const &population) {

    std::ofstream file(filename);

    for (auto const &individual : population) {
        save_melody(file, individual.get_melody());
    }

    file.close();
}

void save_distance(std::ofstream &file, double distance) {
    file << distance << '\n';
}

void save_objective_function(std::string const &filename, std::vector<melody> const &population) {

    std::ofstream file(filename);

    for (auto const &individual : population) {
        save_distance(file, individual.get_distance());
    }

    file.close();
}

void save_emotions(std::string const &filename, std::vector<melody> const &population) {

    std::ofstream file(filename);

    for (auto const &individual : population) {
        file << individual.get_valence_arousal().first  << ' '
             << individual.get_valence_arousal().second << '\n';
    }

    file.close();
}

void save_best_individual (std::string const &filename,
                           std::pair<melody,size_t> const & best_individual) {

    std::ofstream file(filename);

    file << "generation " << best_individual.second << '\n';
    save_melody(file, best_individual.first.get_melody());
    file << best_individual.first.get_distance() << '\n';

    file.close();
}

void save_time(std::string const &filename, std::vector<double> const &execution_times) {

    std::ofstream file(filename);

    for (auto const &time : execution_times) {
        file << time << '\n';
    }

    file.close();
}

void save_parameters(std::string const &filename, double crossover_strength,
                     double mutation_strength, int population_size,
                     size_t max_iterations, double valence_target,
                     double arousal_target) {

    std::ofstream file(filename);

    file << "--crossover_strength " << crossover_strength << ' ';
    file << "--mutation_strength "  << mutation_strength  << ' ';
    file << "--population_size "    << population_size    << ' ';
    file << "--max_iterations "     << max_iterations     << ' ';
    file << "--target "             << valence_target     << ' '
                                    << arousal_target     << ' ';

    file.close();
}

int main () {

    int population_size = 635;
    double mutation_strength = 0.0892;
    double crossover_strength = 0.445;
    int max_iterations = 971;
    std::pair<double, double> target = {0.5,-0.5};

    const melody_problem problem(target, melody_problem::problem_type::twinkle);

    // Variables to save results
    std::vector<double> execution_times;
    double execution_time;
    clock_t initial_time;
    clock_t final_time;

    composer::genetic_algorithm ga(crossover_strength, mutation_strength, population_size, max_iterations, problem);

    save_population("initial-population.txt", ga.get_population());

    for (size_t i = 0; i < 100; ++i) {

        initial_time = clock();

        ga.optimizer();

        final_time = clock();

        execution_time = static_cast<double>(final_time - initial_time)/CLOCKS_PER_SEC;

        execution_times.emplace_back(execution_time);

        // Half evolution
        save_population("execution-" + std::to_string(i+1) +
                            "-half-pop-melodies.txt", ga.get_half_evolution());
        save_objective_function("execution-" + std::to_string(i+1) +
                                    "-half-pop-objectives.txt", ga.get_half_evolution());
        save_emotions("execution-" + std::to_string(i+1) +
                          "-half-pop-emotions.txt", ga.get_half_evolution());

        // Final result
        save_population("execution-" + std::to_string(i+1) +
                            "-pop-melodies.txt", ga.get_population());
        save_objective_function("execution-" + std::to_string(i+1) +
                                    "-pop-objectives.txt", ga.get_population());
        save_emotions("execution-" + std::to_string(i+1) +
                          "-pop-emotions.txt", ga.get_population());

        save_best_individual("execution-" + std::to_string(i+1) +
                                 "-best-individual.txt", ga.get_best_individual());

        save_objective_function("execution-" + std::to_string(i+1) +
                                    "-best_individuals.txt", ga.get_best_individuals());
    }

    save_parameters("parameters.txt", crossover_strength, mutation_strength,
                    population_size, max_iterations, target.first, target.second);
    save_time("execution-times.txt", execution_times);

    ga.display();

    return 0;
}