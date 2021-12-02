//
// Created by Carla on 02/12/2021.
//

#include <iostream>
#include <vector>
#include <composer/melody.h>
#include <map>

using namespace composer;

enum class problem {min_min, max_max, min_max, max_min};

static std::default_random_engine generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

std::vector<int> c_major_double() {
    return {60, 62, 64, 65, 67, 69, 71, 72, 60, 62, 64, 65, 67, 69, 71, 72};
}

void init_population(size_t size, std::vector<std::vector<int>> &population) {

    for (size_t i = 0; i < size; ++i) {
        population.emplace_back(c_major_double());
    }
}

double weighted_sum(double valence, double arousal, problem type, double lambda_1, double lambda_2) {

    switch(type) {
    case problem::max_max:
        valence *= -1;
        arousal *= -1;
        break;

    case problem::min_max:
        arousal *= -1;
        break;

    case problem::max_min:
        valence *= -1;
        break;

    default: // problem::min_min by default
        break;
    }

    return (lambda_1 * valence) + (lambda_2 * arousal);
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

void evaluate_population(std::vector<std::pair<double, int>> &population_values,
                         const std::vector<std::vector<int>> &population_melody, double max_value,
                         problem type, double lambda_1, double lambda_2) {

    std::pair<double,double> evaluation;

    for (size_t i = 0; i < population_melody.size(); ++i) {

        evaluation = evaluate_individual(population_melody[i], max_value);
        population_values.emplace_back(std::pair(
            weighted_sum(evaluation.first, evaluation.second, type, lambda_1, lambda_2),
            static_cast<int>(i))
       );
    }
}

void select_parents(int &parent_1, int &parent_2, size_t population_size) {
    std::uniform_int_distribution<int> d(0, (static_cast<int>(population_size)-1));
    parent_1 = d(generator_);
    parent_2 = d(generator_);
}

bool compare(const std::pair<double,int> &a, const std::pair<double,int> &b) {
    return a.first < b.first;
}

std::pair<std::vector<std::vector<int>>, std::vector<std::pair<double, int>>> genetic_algorithm(
    size_t population_size, size_t individual_size, double mutation_strength,
    int max_iterations, double lambda_1, double lambda_2, problem type) {

    std::vector<std::vector<int>> population_melody;
    std::vector<std::pair<double, int>> population_values;

    init_population(population_size, population_melody);

    std::vector<int> candidate_solution(individual_size);

    std::vector<std::vector<int>> new_population_melody(population_size);

    std::pair<double,double> evaluation;

    int parent_1;
    int parent_2;

    evaluate_population(population_values, population_melody, static_cast<double>(individual_size),
                        type, lambda_1, lambda_2);

    for (int j = 0; j < max_iterations; ++j) {
        for (int i = 0; i < population_size; ++i) {

            // Parents selection
            select_parents(parent_1, parent_2, population_size);

            // Crossover
            candidate_solution = melody::crossover(population_melody[parent_1], population_melody[parent_2]);

            // Mutation
            melody::simple_mutation(candidate_solution, mutation_strength);

            population_melody.emplace_back(candidate_solution);

            // Evaluation
            evaluation = evaluate_individual(candidate_solution, static_cast<double>(individual_size));
            population_values.emplace_back(std::pair(
                weighted_sum(evaluation.first, evaluation.second, type, lambda_1, lambda_2),
                (i+population_size))
            );
        }

        // Parents substitution
        std::ranges::sort(population_values.begin(), population_values.end(), compare);
        population_values.erase(population_values.begin()+static_cast<int>(population_size),population_values.end());

        for (size_t i = 0; i < population_size; ++i) {
            new_population_melody[i] = population_melody[population_values[i].second];
        }

        population_melody.clear();
        population_melody = new_population_melody;
    }

    return {population_melody,population_values};
}

int main () {

    size_t population_size = 100;
    size_t individual_size = 16;
    double mutation_strength = 0.05;
    int max_iterations = 100;

    double lambda_1 = 0.5;
    double lambda_2 = 0.5;
    problem type = problem::min_min;

    std::pair<std::vector<std::vector<int>>, std::vector<std::pair<double, int>>> melody_values;

    melody_values = genetic_algorithm(population_size, individual_size, mutation_strength, max_iterations, lambda_1, lambda_2, type);

    // Save results
    std::ofstream file("experiment_result_min-min.txt");

    file << "POPULATION SIZE: " << population_size << "\n";
    file << "INDIVIDUAL SIZE: " << individual_size << "\n";
    file << "MAX ITERATIONS: " << max_iterations << "\n";
    file << "MUTATION STRENGTH: " << mutation_strength << "\n";
    file << "LAMBDA 1: : " << lambda_1 << "\n";
    file << "LAMBDA 2: : " << lambda_2 << "\n";
    file << "PROBLEM TYPE: MIN MIN \n\n";

    file << "FINAL MELODIES\n";

    for (const auto &melody_ : melody_values.first) {
        file << "[";
        for (const int &note : melody_) {
            file << note << " ";
        }
        file << "]\n";
    }

    file << "\nPOPULATION VALUES\n";

    for (const auto& [key, value] : melody_values.second) {
        file << "[" << key << " ";
        file << "]\n";
    }

    file.close();

    return 0;
}