//
// Created by Carla on 20/12/2021.
//

#ifndef COMPOSER_MELODY_PROBLEM_H
#define COMPOSER_MELODY_PROBLEM_H

#include <utility>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <MidiFile.h>

namespace composer {
    class melody_problem {
      public:

        enum class problem_type {c_major_double, twinkle, random, one_note};

        explicit melody_problem(const smf::MidiFile &midi_file, std::pair<double,double> target = {0., 0.})
            : target_(std::move(target)) {
            this->type_ = problem_type::random;
            this->rhythm_ = midi_file.getTicksPerQuarterNote();
            this->melody_ = import_melody(midi_file[0]);
        };

        melody_problem(problem_type type, int rhythm, std::pair<double,double> target = {0., 0.}, size_t size = 16)
            : target_(std::move(target)),
              type_(type),
              rhythm_(rhythm) {

            switch (type) {

                case problem_type::c_major_double:
                    this->melody_ = c_major_double();
                    break;

                case problem_type::twinkle:
                    this->melody_ = twinkle();
                    break;

                case problem_type::random:
                    this->melody_ = random_problem(size);
                    break;

                case problem_type::one_note:
                    this->melody_ = one_note(size);
                    break;

                default:
                    break;
            }
        };

        [[nodiscard]] std::vector<std::pair<int,int>> get_melody() const;
        void set_melody(std::vector<std::pair<int,int>> const &new_melody);

        [[maybe_unused]] [[nodiscard]] int get_rhythm() const;
        [[maybe_unused]] void set_rhythm(const int &new_rhythm);

        static std::vector<std::pair<int,int>> c_major_double();
        static std::vector<std::pair<int,int>> twinkle();
        static std::vector<std::pair<int,int>> random_problem(size_t size);
        static std::vector<std::pair<int,int>> one_note(size_t size);
        static std::vector<std::pair<int,int>> import_melody(smf::MidiEventList event_list);

        [[nodiscard]] std::pair<double,double> get_target() const;
        [[maybe_unused]] void set_target(std::pair<double,double> &new_target);

        [[nodiscard]] problem_type get_type() const;

      private:
        std::vector<std::pair<int,int>> melody_;
        int rhythm_;
        std::pair<double,double> target_;
        problem_type type_;

        static std::default_random_engine generator_;
    };
} // namespace composer

#endif // COMPOSER_MELODY_PROBLEM_H
