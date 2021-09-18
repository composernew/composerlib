//
// Created by Alan Freitas on 12/14/20.
//

#ifndef COMPOSER_MELODY_H
#define COMPOSER_MELODY_H

#include <vector>
#include <MidiFile.h>
#include <random>
#include <iostream>
#include <chrono>
#include <tuple>
#include <algorithm>

namespace composer {

    /// \brief Melody optimization.
    ///
    /// This class contains functions to generate melodies with a genetic algorithm.

    class melody {
      public:

        /// \brief Creates a random melody as an initial problem.
        ///
        /// The melody created is a vector of vector of integers that represents
        /// the population with all of your individuals in a genetic algorithm.
        /// So, the solution will be a matrix of notes which each line is a
        /// measure and each measure is a set of notes.
        ///
        /// \param problem_size The number of individuals of the problem.
        /// \param individual_size The individual size. Each individual is a vector of integers.
        explicit melody(const size_t &problem_size, const size_t &individual_size);

        /// Displays the current state of the melody in the console.
        void display();

        /// \brief An auxiliary function that creates an individual.
        ///
        /// An individual is a vector of integers with a fixed size that
        /// represents a measure with musical notes.
        static std::vector<int> create_individual(const size_t &individual_size);

        /// \brief The crossover operation.
        ///
        /// \param parent A vector of integers that is an individual from previous generation.
        /// \return A vector of integers that represents an individual that is a child of the
        /// individual passed as parameter.
        static std::vector<int> crossover(std::vector<int> &parent);

        /// \brief The simple_mutation operator.
        ///
        /// This operator increases some values in the individual by one depending
        /// on the mutation strength provided.
        ///
        /// \param individual The individual that has to be modified.
        /// \param mutation_strength A double value that is used as parameter to
        /// define the possibility of a value in an individual being modified.
        static void simple_mutation(std::vector<int> &individual, double mutation_strength);

        /// \brief The reverse_measure mutation operator.
        ///
        /// This operator reverses the order of all individual values depending
        /// on the mutation strength provided. Note that its different from
        /// the simple mutation, changing all positions of values.
        ///
        /// \param individual The individual that has to be modified.
        /// \param mutation_strength A double value that is used as parameter to
        /// define the possibility of a value in an individual being modified.
        static void reverse_measure(std::vector<int> &individual, double mutation_strength);

        /// \brief The exchange_pulses mutation operator.
        ///
        /// This operator exchanges two selected values of individuals depending
        /// on the mutation strength provided.
        ///
        /// \param individual The individual that has to be modified.
        /// \param mutation_strength A double value that is used as parameter to
        /// define the possibility of a value in an individual being modified.
        static void exchange_pulses(std::vector<int> &individual, double mutation_strength);

        /// \brief The reverse_pulses mutation operator.
        ///
        /// This operator reverses a range between two selected values of
        /// individuals depending on the mutation strength provided.
        ///
        /// \param individual The individual that has to be modified.
        /// \param mutation_strength A double value that is used as parameter to
        /// define the possibility of a value in an individual being modified.
        static void reverse_pulses(std::vector<int> &individual, double mutation_strength);

        /// \brief The most used values in an individual.
        static double evaluate_pitch_distribution(std::vector<int> &individual);

        static double evaluate_pitch_variety(std::vector<int> individual);

        /// \brief An evaluate function to determine the valence and arousal values of
        /// an individual.
        ///
        /// The value generated by the evaluate function is to be defined.
        ///
        /// \param individual The individual that is evaluated.
        /// \return A tuple with the valence and arousal values of the individual
        /// passed as parameter.
        static std::tuple<double,double> evaluate(std::vector<int> &individual);

        static double normalize(int &value, double max, double min, double max_value, double min_value);

        std::vector<std::vector<int>> get_melody();
        void add_individual(std::vector<int> individual);
        void clear();

      private:
        std::vector<std::vector<int>> melody_;        ///< The melody to be optimized.
        static std::default_random_engine generator_; ///< Used to generate random values and create the melody.
    };
} // namespace composer

#endif // COMPOSER_MELODY_H
