//
// Created by Carla on 20/12/2021.
//

#ifndef COMPOSER_MELODY_PROBLEM_H
#define COMPOSER_MELODY_PROBLEM_H

#include <algorithm>
#include <chrono>
#include <random>
#include <utility>
#include <vector>

namespace composer {
    class melody_problem {
      public:
        enum class problem_type { c_major_double, twinkle, random, one_note };

        explicit melody_problem(problem_type type = problem_type::random, int rhythm = 120,
                       std::pair<double, double> target = {0., 0.},
                       size_t size = 16,
                       int lowest_pitch = 21,
                       int highest_pitch = 108,
                       int quietest_volume = 0,
                       int loudest_volume = 127,
                       int shortest_duration = 0,
                       int longest_duration = 100,
                       int slowest_tempo = 40,
                       int fastest_tempo = 208)
            : target_(std::move(target)), type_(type), rhythm_(rhythm),
              lowest_pitch_(lowest_pitch),
              highest_pitch_(highest_pitch),
              quietest_volume_(quietest_volume),
              loudest_volume_(loudest_volume),
              shortest_duration_(shortest_duration),
              longest_duration_(longest_duration),
              slowest_tempo_(slowest_tempo),
              fastest_tempo_(fastest_tempo) {

            std::uniform_int_distribution d(
                static_cast<int>(this->slowest_tempo_),
                static_cast<int>(this->fastest_tempo_));

            switch (type) {

                case problem_type::c_major_double:
                    this->melody_ = c_major_double();
                    break;

                case problem_type::twinkle:
                    this->melody_ = twinkle();
                    break;

                case problem_type::random:
                    this->melody_ = random_problem(size);
                    this->rhythm_ = d(generator_);
                    break;

                case problem_type::one_note:
                    this->melody_ = one_note(size);
                    break;

                default:
                    break;
            }
        };

        [[nodiscard]] std::vector<std::tuple<int,int,int>> get_melody() const;
        void set_melody(std::vector<std::tuple<int,int,int>> const &new_melody);

        [[maybe_unused]] [[nodiscard]] int get_rhythm() const;
        [[maybe_unused]] void set_rhythm(const int &new_rhythm);

        static std::vector<std::tuple<int,int,int>> c_major_double();
        static std::vector<std::tuple<int,int,int>> twinkle();
        [[nodiscard]] std::vector<std::tuple<int,int,int>> random_problem(size_t size) const;
        [[nodiscard]] std::vector<std::tuple<int,int,int>> one_note(size_t size) const;

        [[nodiscard]] std::pair<double,double> get_target() const;
        [[maybe_unused]] void set_target(std::pair<double,double> &new_target);

        [[nodiscard]] problem_type get_type() const;

        [[nodiscard]] int get_lowest_pitch() const;

        [[nodiscard]] int get_highest_pitch() const;

        [[nodiscard]] int get_quietest_volume() const;

        [[nodiscard]] int get_loudest_volume() const;

        [[nodiscard]] int get_shortest_duration() const;

        [[nodiscard]] int get_longest_duration() const;

        [[nodiscard]] int get_slowest_tempo() const;

        [[nodiscard]] int get_fastest_tempo() const;

        static int get_pause();

      private:
        std::vector<std::tuple<int,int,int>> melody_;
        int rhythm_;
        std::pair<double,double> target_;
        problem_type type_;

        static constexpr int pause = 20;
        int lowest_pitch_;
        int highest_pitch_;
        int quietest_volume_;
        int loudest_volume_;
        int shortest_duration_;
        int longest_duration_;
        int slowest_tempo_;
        int fastest_tempo_;

        static std::default_random_engine generator_;
    };
} // namespace composer

#endif // COMPOSER_MELODY_PROBLEM_H
