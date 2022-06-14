//
// Created by Alan Freitas on 12/14/20.
//

#ifndef COMPOSER_MELODY_H
#define COMPOSER_MELODY_H

#include <vector>
#include <random>
#include <iostream>
#include <chrono>
#include <tuple>
#include <algorithm>
#include "melody_problem.h"

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
        explicit melody(melody_problem const &problem);

        melody();

        /// \brief The most used values in an individual.
        double evaluate_pitch_distribution();

        [[nodiscard]] double evaluate_pitch_variety() const;

        static double normalize(double value, double max, double min, double max_value, double min_value);

        void evaluate();

        static double euclidean_distance(std::pair<double,double> const &p1, std::pair<double,double> const &p2);

        /// \brief The simple_mutation operator.
        ///
        /// This operator increases some values in the individual by one depending
        /// on the mutation strength provided.
        ///
        /// \param individual The individual that has to be modified.
        /// \param mutation_strength A double value that is used as parameter to
        /// define the possibility of a value in an individual being modified.
        void simple_mutation();

        /// \brief The reverse_measure mutation operator.
        ///
        /// This operator reverses the order of all individual values depending
        /// on the mutation strength provided. Note that its different from
        /// the simple mutation, changing all positions of values.
        ///
        /// \param individual The individual that has to be modified.
        /// \param mutation_strength A double value that is used as parameter to
        /// define the possibility of a value in an individual being modified.
        void reverse_measure();

        /// \brief The exchange_pulses mutation operator.
        ///
        /// This operator exchanges two selected values of individuals depending
        /// on the mutation strength provided.
        ///
        /// \param individual The individual that has to be modified.
        /// \param mutation_strength A double value that is used as parameter to
        /// define the possibility of a value in an individual being modified.
        void exchange_pulses();

        /// \brief The reverse_pulses mutation operator.
        ///
        /// This operator reverses a range between two selected values of
        /// individuals depending on the mutation strength provided.
        ///
        /// \param individual The individual that has to be modified.
        /// \param mutation_strength A double value that is used as parameter to
        /// define the possibility of a value in an individual being modified.
        void reverse_pulses();

        /// \brief The crossover operation.
        ///
        /// \param parent A vector of integers that is an individual from previous generation.
        /// \return A vector of integers that represents an individual that is a child of the
        /// individual passed as parameter.
        static melody crossover(const melody &first_parent, const melody &second_parent);

        [[nodiscard]] std::vector<int> get_melody() const;

        [[nodiscard]] double get_distance() const;

        [[nodiscard]] std::pair<double,double> get_valence_arousal() const;

        double get_rhythm() const;

        void set_valence_arousal(double valence, double arousal);

        void set_valence_arousal(std::pair<double,double> new_valence_arousal);

        void set_melody(int note);

        void set_rhythm(double new_rhythm);

        void set_distance(double new_distance);

        friend std::ostream &operator<<(std::ostream &os,
                                        const melody &individual) {

            for (auto const &note : individual.get_melody())
                os << note << ' ';

            return os;
        }

      private:

        std::vector<int> melody_;
        std::pair<double,double> valence_arousal;
        double rhythm;
        double distance;

        static std::default_random_engine generator_;
    };
} // namespace composer

#endif // COMPOSER_MELODY_H
