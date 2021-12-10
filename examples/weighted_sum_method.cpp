//
// Created by Carla on 02/12/2021.
//

#include <iostream>
#include <vector>
#include <composer/melody.h>
#include <cmath>
#include <ctime>

using namespace composer;

static std::default_random_engine generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

std::vector<int> c_major_double() {
    return {60, 62, 64, 65, 67, 69, 71, 72, 60, 62, 64, 65, 67, 69, 71, 72};
}

double euclidian_distance(std::pair<double,double> p1, std::pair<double,double> p2) {
    return sqrt(pow((p1.first - p2.first), 2) + (pow((p1.second - p2.second),2)));
}

std::pair<double,double> evaluate_individual(const std::vector<int> &individual, double max_value) {

    double normalized_pitch_variety =
        melody::normalize(melody::evaluate_pitch_variety(individual), 1,
                                    -1, max_value, 1.);
    double normalized_pitch_distribution =
        melody::normalize(melody::evaluate_pitch_distribution(individual), 1.,
                                    -1., 108., 20);

    double valence = normalized_pitch_variety;
    double arousal = (normalized_pitch_variety + normalized_pitch_distribution)/2.;

    return {valence,arousal};
}

melody init_melody(const std::pair<double,double> &emotion_target, const std::vector<int> &new_melody) {

    melody m(new_melody);
    m.set_valence_arousal(evaluate_individual(m.get_melody(), m.get_melody().size()));
    m.set_distance(euclidian_distance(emotion_target, m.get_valence_arousal()));

    return m;
}

void init_population(size_t size, const std::pair<double,double> &emotion_target, std::vector<melody> &population) {

    for (size_t i = 0; i < size; ++i) {
        population.emplace_back(init_melody(emotion_target, c_major_double()));
    }
}

void select_parents(int &parent_1, int &parent_2, size_t population_size) {
    std::uniform_int_distribution<int> d(0, (static_cast<int>(population_size)-1));
    parent_1 = d(generator_);
    parent_2 = d(generator_);
}

void select_mutation(std::vector<int> &individual, double mutation_strength) {

    std::uniform_real_distribution<double> real_d(0.0, 1.0);

    if (real_d(generator_) < mutation_strength) {
        std::uniform_int_distribution<int> d(1, 4);

        int mutation = d(generator_);

        switch (mutation) {
        case 1:
            melody::simple_mutation(individual);
            break;
        case 2:
            melody::reverse_measure(individual);
            break;
        case 3:
            melody::exchange_pulses(individual);
        default:
            melody::reverse_pulses(individual);
            break;
        }
    }
}

bool compare(melody &a, melody &b) {
    return a.get_distance() < b.get_distance();
}

std::tuple<std::vector<melody>, double, double, double, double, double>
    genetic_algorithm( size_t population_size, size_t individual_size,
                  double mutation_strength, int max_iterations, std::pair<double,double> emotion_target) {

    std::ofstream file("evolution.txt");

    std::vector<melody> population;

    std::vector<int> child(individual_size);

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
            child = melody::crossover(population[parent_1].get_melody(),
                                      population[parent_2].get_melody());

            // Mutation
            select_mutation(child, mutation_strength);

            // Evaluation and population
            population.emplace_back(init_melody(emotion_target, child));
        }

        // Parents substitution
        std::sort(population.begin(), population.end(), compare);
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

    for (melody &melody_ : std::get<0>(results[best_execution])) {
        file << "[";
        for (const int &note : melody_.get_melody()) {
            file << note << " ";
        }
        file << "]\n";
    }

    file << "\nPOPULATION DISTANCE\n";

    for (melody &melody_ : std::get<0>(results[best_execution])) {
        file << "[" << melody_.get_distance() << "]\n";
    }

    file << "\nPOPULATION VALUES\n";

    for (melody &melody_ : std::get<0>(results[best_execution])) {
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
        std::cout << "Exec. " << i << "\n";
        result = genetic_algorithm(population_size, individual_size, mutation_strength, max_iterations, target);
        results.emplace_back(result);
    }

    save_results(results, population_size, individual_size, mutation_strength, max_iterations);

    return 0;
}