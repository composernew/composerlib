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

        // pesquisar como esses parâmetros podem ser setados apenas uma vez. melody_problem é único e o usuário só deve poder
        // setar valores na hora de declarar.
        melody_problem(problem_type type, int rhythm = 120,
                           std::pair<double, double> target = {0., 0.},
                           size_t size = 16,
                           int lowest_pitch = 21,
                           int highest_pitch = 108,
                           int quietest_volume = 0,
                           int loudest_volume = 127,
                           int longest_duration = 0,
                           int shortest_duration = 100,
                           int slowest_tempo = 40,
                           int fastest_tempo = 208
                       )
            : rhythm_(rhythm), target_(std::move(target)), type_(type),
              lowest_pitch_(lowest_pitch),
              highest_pitch_(highest_pitch),
              quietest_volume_(quietest_volume),
              loudest_volume_(loudest_volume),
              shortest_duration_(shortest_duration),
              longest_duration_(longest_duration),
              slowest_tempo_(slowest_tempo),
              fastest_tempo_(fastest_tempo) {

            std::uniform_int_distribution d(this->slowest_tempo_, this->fastest_tempo_);

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

        static std::vector<std::tuple<int,int,int>> c_major_double();
        static std::vector<std::tuple<int,int,int>> twinkle();
        std::vector<std::tuple<int,int,int>> random_problem(size_t size);
        std::vector<std::tuple<int,int,int>> one_note(size_t size);

        [[nodiscard]] std::pair<double,double> get_target() const;

        [[nodiscard]] problem_type get_type() const;

        int get_lowest_pitch() const;

        int get_highest_pitch() const;

        int get_quietest_volume() const;

        int get_loudest_volume() const;

        int get_shortest_duration() const;

        int get_longest_duration() const;

        int get_slowest_tempo() const;

        int get_fastest_tempo() const;

        static int get_pause();

      private:
        std::vector<std::tuple<int,int,int>> melody_;
        int rhythm_;
        const std::pair<double,double> target_;
        const problem_type type_;

        static constexpr int pause = 20;
        const int lowest_pitch_;
        const int highest_pitch_;
        const int quietest_volume_;
        const int loudest_volume_;
        const int shortest_duration_;
        const int longest_duration_;
        const int slowest_tempo_;
        const int fastest_tempo_;

        static std::default_random_engine generator_;
    };
} // namespace composer

#endif // COMPOSER_MELODY_PROBLEM_H
