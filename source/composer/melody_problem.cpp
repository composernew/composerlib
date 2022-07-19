//
// Created by Carla on 21/12/2021.
//

#include "melody_problem.h"
#include <sstream>
#include <iostream>
#include <utility>

namespace composer {

    std::default_random_engine melody_problem::generator_ = std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count());

    std::vector<std::pair<int,int>> melody_problem::c_major_double() {
        return {{60, 50},
                {62, 50},
                {64, 50},
                {65, 50},
                {67, 50},
                {69, 50},
                {71, 50},
                {72, 50},
                {60, 50},
                {62, 50},
                {64, 50},
                {65, 50},
                {67, 50},
                {69, 50},
                {71, 50},
                {72, 50}};
    }

    std::vector<std::pair<int,int>> melody_problem::twinkle() {
        return {{53, 50},
                {53, 50},
                {60, 50},
                {60, 50},
                {62, 50},
                {62, 50},
                {60, 50},
                {20, 50},
                {58, 50},
                {58, 50},
                {57, 50},
                {57, 50},
                {55, 50},
                {55, 50},
                {53, 50},
                {20, 50},
                {60, 50},
                {60, 50},
                {58, 50},
                {58, 50},
                {57, 50},
                {57, 50},
                {55, 50},
                {20, 50},
                {60, 50},
                {60, 50},
                {58, 50},
                {58, 50},
                {57, 50},
                {57, 50},
                {55, 50},
                {20, 50},
                {53, 50},
                {53, 50},
                {60, 50},
                {60, 50},
                {62, 50},
                {62, 50},
                {60, 50},
                {20, 50},
                {58, 50},
                {58, 50},
                {57, 50},
                {57, 50},
                {55, 50},
                {55, 50},
                {53, 50},
                {20, 50}};
    }

    std::vector<std::pair<int,int>> melody_problem::one_note(size_t size) {
        std::vector<std::pair<int,int>> melody;
        std::uniform_int_distribution d(20, 108);
        std::uniform_int_distribution r(10, 100);
        double duration = r(generator_);
        int note = d(generator_);

        for (size_t i = 0; i < size; ++i) {
            melody.emplace_back(std::make_pair(note, duration));
        }

        return melody;
    }

    std::vector<std::pair<int,int>> melody_problem::random_problem(size_t size) {

        std::vector<std::pair<int,int>> melody;
        std::uniform_int_distribution d(20, 108);
        std::uniform_int_distribution r(10, 100);

        for (size_t i = 0; i < size; ++i) {
            melody.emplace_back(std::make_pair(d(generator_), r(generator_)));
        }

        return melody;
    }

    std::vector<std::pair<int,int>> melody_problem::import_melody(smf::MidiEventList event_list) {

        std::vector<std::pair<int,int>> melody;
        std::stringstream stream_note;
        int note = 0;
        int pause = 0;
        int duration = 0;

        if (event_list.size() != 0) {
            for (int event = 1; event < event_list.size(); ++event) {

                if (event_list[event].isNoteOn()) {

                    if (event == 1) duration = static_cast<int>(event_list[event+1].tick);
                    else duration = static_cast<int>(event_list[event].tick) -
                                   static_cast<int>(event_list[event-2].tick);

                    if (event > 2) {
                        pause = static_cast<int>(event_list[event].tick) - static_cast<int>(event_list[event-1].tick);
                        if (pause > 0) melody.emplace_back(std::make_pair(20, pause));
                    }

                    stream_note << std::hex << static_cast<int>(event_list[event][1]);
                    stream_note >> note;
                    melody.emplace_back(std::make_pair(note, (duration-pause)));

                    stream_note.str(std::string());
                    stream_note.clear();
                }
            }
        }

        return melody;
    }

    std::vector<std::pair<int,int>> composer::melody_problem::get_melody() const {
        return this->melody_;
    }

    void melody_problem::set_melody(const std::vector<std::pair<int,int>> &new_melody) {
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

