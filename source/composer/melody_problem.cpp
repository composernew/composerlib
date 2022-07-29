//
// Created by Carla on 21/12/2021.
//

#include "melody_problem.h"
#include <sstream>
#include <iostream>
#include <utility>

namespace composer {

    std::default_random_engine melody_problem::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    std::vector<std::tuple<int,int,int>> melody_problem::c_major_double() {
        return {{60, 50, 80},
                {62, 50, 80},
                {64, 50, 80},
                {65, 50, 80},
                {67, 50, 80},
                {69, 50, 80},
                {71, 50, 80},
                {72, 50, 80},
                {60, 50, 80},
                {62, 50, 80},
                {64, 50, 80},
                {65, 50, 80},
                {67, 50, 80},
                {69, 50, 80},
                {71, 50, 80},
                {72, 50, 80}};
    }

    std::vector<std::tuple<int,int,int>> melody_problem::twinkle() {
        return {{53, 50, 80},
                {53, 50, 80},
                {60, 50, 80},
                {60, 50, 80},
                {62, 50, 80},
                {62, 50, 80},
                {60, 50, 80},
                {20, 50, 80},
                {58, 50, 80},
                {58, 50, 80},
                {57, 50, 80},
                {57, 50, 80},
                {55, 50, 80},
                {55, 50, 80},
                {53, 50, 80},
                {20, 50, 80},
                {60, 50, 80},
                {60, 50, 80},
                {58, 50, 80},
                {58, 50, 80},
                {57, 50, 80},
                {57, 50, 80},
                {55, 50, 80},
                {20, 50, 80},
                {60, 50, 80},
                {60, 50, 80},
                {58, 50, 80},
                {58, 50, 80},
                {57, 50, 80},
                {57, 50, 80},
                {55, 50, 80},
                {20, 50, 80},
                {53, 50, 80},
                {53, 50, 80},
                {60, 50, 80},
                {60, 50, 80},
                {62, 50, 80},
                {62, 50, 80},
                {60, 50, 80},
                {20, 50, 80},
                {58, 50, 80},
                {58, 50, 80},
                {57, 50, 80},
                {57, 50, 80},
                {55, 50, 80},
                {55, 50, 80},
                {53, 50, 80},
                {20, 50, 80}};
    }

    std::vector<std::tuple<int,int,int>> melody_problem::one_note(size_t size) {

        std::vector<std::tuple<int,int,int>> melody;

        std::uniform_int_distribution d(static_cast<int>(feature_type::lowest_duration),
                                        static_cast<int>(feature_type::highest_duration));
        std::uniform_int_distribution v(static_cast<int>(feature_type::lowest_volume),
                                        static_cast<int>(feature_type::highest_volume));
        std::uniform_int_distribution n(static_cast<int>(feature_type::pause),
                                        static_cast<int>(feature_type::highest_pitch));

        double duration = d(generator_);
        double volume   = v(generator_);
        int note        = n(generator_);

        for (size_t i = 0; i < size; ++i) {
            melody.emplace_back(std::make_tuple(note, duration, volume));
        }

        return melody;
    }

    std::vector<std::tuple<int,int,int>> melody_problem::random_problem(size_t size) {

        std::vector<std::tuple<int,int,int>> melody;

        std::uniform_int_distribution d(static_cast<int>(feature_type::lowest_duration),
                                        static_cast<int>(feature_type::highest_duration));
        std::uniform_int_distribution v(static_cast<int>(feature_type::lowest_volume),
                                        static_cast<int>(feature_type::highest_volume));
        std::uniform_int_distribution n(static_cast<int>(feature_type::pause),
                                        static_cast<int>(feature_type::highest_pitch));

        for (size_t i = 0; i < size; ++i) {
            melody.emplace_back(std::make_tuple(n(generator_), d(generator_), v(generator_)));
        }

        return melody;
    }

    std::vector<std::tuple<int,int,int>> melody_problem::import_melody(smf::MidiEventList event_list) {

        std::vector<std::tuple<int,int,int>> melody;
        std::stringstream stream_note;
        int note = 0;
        int pause_duration = 0;
        int duration = 0;
        int velocity = 0;

        if (event_list.size() != 0) {
            for (int event = 0; event < event_list.size(); ++event) {

                if (event_list[event].isNoteOn()) {

                    if (melody.empty()) duration = static_cast<int>(event_list[event+1].tick);
                    else duration = static_cast<int>(event_list[event].tick) -
                                   static_cast<int>(event_list[event-2].tick);

                    if (!melody.empty()) {
                        pause_duration = static_cast<int>(event_list[event].tick) - static_cast<int>(event_list[event-1].tick);
                        if (pause_duration > 5) {
                            melody.emplace_back(std::make_tuple(static_cast<int>(feature_type::pause), pause_duration, 0));
                        }
                    }

                    velocity = static_cast<int>(event_list[event].getVelocity());

                    stream_note << std::hex << static_cast<int>(event_list[event][1]);
                    stream_note >> note;
                    melody.emplace_back(std::make_tuple(note, (duration- pause_duration), velocity));

                    stream_note.str(std::string());
                    stream_note.clear();
                }
            }
        }

        return melody;
    }

    std::vector<std::tuple<int,int,int>> composer::melody_problem::get_melody() const {
        return this->melody_;
    }

    void melody_problem::set_melody(const std::vector<std::tuple<int,int,int>> &new_melody) {
        this->melody_ = new_melody;
    }

    std::pair<double, double> melody_problem::get_target() const {
        return this->target_;
    }

    melody_problem::problem_type melody_problem::get_type() const {
        return this->type_;
    }

    [[maybe_unused]] int melody_problem::get_rhythm() const {
        return this->rhythm_;
    }

    [[maybe_unused]] void melody_problem::set_rhythm(const int &new_rhythm) {
        this->rhythm_ = new_rhythm;
    }

    [[maybe_unused]] void melody_problem::set_target(std::pair<double,double> &new_target) {
        this->target_ = new_target;
    }
} // namespace composer

