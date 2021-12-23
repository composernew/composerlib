//
// Created by Carla on 02/12/2021.
//

#include <iostream>
#include <vector>
#include "composer/melody.h"
#include "composer/melody_problem.h"
#include <cmath>
#include <ctime>

using namespace composer;

static std::default_random_engine generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

const melody_problem problem({0.5, 0.5});

void init_population(size_t size, std::vector<melody> &population) {

    for (size_t i = 0; i < size; ++i) {
        melody m(problem);
        population.emplace_back(m);
    }
}

void select_parents(int &parent_1, int &parent_2, size_t population_size) {
    std::uniform_int_distribution d(0, (static_cast<int>(population_size)-1));
    parent_1 = d(generator_);
    parent_2 = d(generator_);
}

void select_mutation(melody &individual, double mutation_strength) {

    std::uniform_real_distribution real_d(0.0, 1.0);

    if (real_d(generator_) < mutation_strength) {
        std::uniform_int_distribution d(1, 4);

        int mutation = d(generator_);

        switch (mutation) {
        case 1:
            individual.simple_mutation();
            break;
        case 2:
            individual.reverse_measure();
            break;
        case 3:
            individual.exchange_pulses();
        default:
            individual.reverse_pulses();
            break;
        }
    }
}

bool compare(const melody &a, const melody &b) {
    return a.get_distance() < b.get_distance();
}

std::tuple<std::vector<melody>, double, double, double, double, double>
    genetic_algorithm( size_t population_size, size_t individual_size,
                  double mutation_strength, int max_iterations, std::pair<double,double> emotion_target) {

    std::ofstream file("evolution.txt");

    std::vector<melody> population;

    melody child;

    int parent_1;
    int parent_2;

    double best_solution = 2;

    clock_t inicio_CPU;       // clock no inicio da aplicacao do metodo
    clock_t fim_CPU;          // clock ao encontrar a melhos solução
    double best_time;

    double easy_goal = 0.2;
    double medium_goal = 0.1;
    double hard_goal = 0.0;
    double easy_time = 0;
    double medium_time = 0;
    double hard_time = 0;

    inicio_CPU = clock();

    init_population(population_size, population);

    for (int j = 0; j < max_iterations; ++j) {
        for (int i = 0; i < population_size; ++i) {

            // Parents selection
            select_parents(parent_1, parent_2, population_size);

            // Crossover
            child = melody::crossover(population[parent_1], population[parent_2]);

            // Mutation
            select_mutation(child, mutation_strength);

            // Evaluation
            child.evaluate();

            // Population
            population.emplace_back(child);
        }

        // Parents substitution
        std::ranges::sort(population.begin(), population.end(), compare);
        population.erase(population.begin()+static_cast<int>(population_size),
                         population.end());

        file << population[0].get_distance() << "\n";

        if (population[0].get_distance() < best_solution) {
            best_solution = population[0].get_distance();
            fim_CPU = clock();
            best_time = (double)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC;

            // Para cálculo da distribuição da probabilidade empírica
            if (best_solution == hard_goal) {
                hard_time = best_time;
                return {population, best_solution, best_time, easy_time, medium_time, hard_time};
            }
            else if (best_solution <= medium_goal) {
                medium_time = best_time;
                return {population, best_solution, best_time, easy_time, medium_time, hard_time};
            }
            else if (best_solution <= easy_goal) {
                easy_time = best_time;
                return {population, best_solution, best_time, easy_time, medium_time, hard_time};
            }
        }
    }

    file.close();

    return {population, best_solution, best_time, easy_time, medium_time, hard_time};
}

void save_results(auto &results, size_t population_size, size_t individual_size,
                  double mutation_strength, int max_iterations) {

    int num_executions = 100;
    double best_solution = 2;
    int best_execution = 0;
    std::ofstream file("teste2.txt");

    for (int i = 0; i < num_executions; ++i) {

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
}

int main () {

    size_t population_size = 500;
    size_t individual_size = 16;
    double mutation_strength = 0.7;
    int max_iterations = 100;
    std::pair<double,double> target = {0.5,0.5};

    std::tuple<std::vector<melody>, double, double, double, double, double> result;
    std::vector<std::tuple<std::vector<melody>, double, double, double, double, double>> results;

    for (int i = 0; i < 100; ++i) {
        result = genetic_algorithm(population_size, individual_size, mutation_strength, max_iterations, target);
        results.emplace_back(result);
    }

    save_results(results, population_size, individual_size, mutation_strength, max_iterations);

    return 0;
}