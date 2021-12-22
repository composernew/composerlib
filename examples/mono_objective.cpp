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

const melody_problem problem;

void init_population(size_t size, const std::pair<double,double> &emotion_target, std::vector<melody> &population) {

    for (size_t i = 0; i < size; ++i) {
        melody m(problem, emotion_target);
        population.emplace_back(m);
    }
}

void select_parents(int &parent_1, int &parent_2, size_t population_size) {
    std::uniform_int_distribution d(0, (static_cast<int>(population_size)-1));
    parent_1 = d(generator_);
    parent_2 = d(generator_);
}

void simple_mutation(melody &individual) {

    std::uniform_int_distribution dist_int(0, 1);

    for (int &item : individual.get_melody()) {
        if(dist_int(generator_)) {
            if (item < 108)
                item = item + 1;
        }
        else {
            if (item > 21)
                item = item - 1;
        }
    }
}

void reverse_measure(melody &individual) {
    std::ranges::reverse(individual.get_melody().begin(), individual.get_melody().end());
}

void exchange_pulses(melody &individual) {

    std::uniform_int_distribution value_distribution(0, static_cast<int>(individual.get_melody().size()-1));
    int first_pulse = value_distribution(generator_);
    int second_pulse = value_distribution(generator_);
    std::swap(individual.get_melody()[first_pulse], individual.get_melody()[second_pulse]);
}

void reverse_pulses(melody &individual) {

    std::uniform_int_distribution first_value_distribution(0, static_cast<int>(individual.get_melody().size()));
    int first_pulse = first_value_distribution(generator_);

    std::uniform_int_distribution second_value_distribution(first_pulse, static_cast<int>(individual.get_melody().size()));
    int second_pulse = second_value_distribution(generator_);

    std::reverse(individual.get_melody().begin() + first_pulse, individual.get_melody().begin() + second_pulse);
}

void select_mutation(melody &individual, double mutation_strength) {

    std::uniform_real_distribution real_d(0.0, 1.0);

    if (real_d(generator_) < mutation_strength) {
        std::uniform_int_distribution d(1, 4);

        int mutation = d(generator_);

        switch (mutation) {
        case 1:
            simple_mutation(individual);
            break;
        case 2:
            reverse_measure(individual);
            break;
        case 3:
            exchange_pulses(individual);
        default:
            reverse_pulses(individual);
            break;
        }
    }
}

bool compare(const melody &a, const melody &b) {
    return a.get_distance() < b.get_distance();
}

melody crossover(const melody &first_parent, const melody &second_parent) {
    std::uniform_int_distribution d(0,1);
    melody child;

    for (size_t i = 0; i < first_parent.get_melody().size(); ++i) {
        if (d(generator_)) {
            child.get_melody().emplace_back(first_parent.get_melody()[i]);
        }
        else {
            child.get_melody().emplace_back(second_parent.get_melody()[i]);
        }
    }

    return child;
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

    init_population(population_size, emotion_target, population);

    for (int j = 0; j < max_iterations; ++j) {
        for (int i = 0; i < population_size; ++i) {

            // Parents selection
            select_parents(parent_1, parent_2, population_size);

            // Crossover
            child = crossover(population[parent_1], population[parent_2]);

            // Mutation
            select_mutation(child, mutation_strength);

            // Evaluation
            melody::evaluate(child);

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
    double mutation_strength = 0.07;
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