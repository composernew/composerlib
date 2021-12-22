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
        explicit melody(melody_problem const &problem, const std::pair<double,double> &target);

        melody();

        /// \brief The most used values in an individual.
        static double evaluate_pitch_distribution(melody const &individual);

        static double evaluate_pitch_variety(melody const &individual);

        static double normalize(double value, double max, double min, double max_value, double min_value);

        static std::pair<double,double> evaluate(melody const &individual);

        static double euclidean_distance(std::pair<double,double> p1, std::pair<double,double> p2);

        [[nodiscard]] std::vector<int> get_melody() const;

        [[nodiscard]] double get_distance() const;

        [[nodiscard]] std::pair<double,double> get_valence_arousal() const;

      private:

        std::vector<int> melody_;
        std::pair<double,double> valence_arousal;
        std::pair<double,double> target_;
        double distance;
    };
} // namespace composer

#endif // COMPOSER_MELODY_H
