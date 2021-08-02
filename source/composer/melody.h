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

namespace composer {
    class melody {
      public:
        explicit melody(const size_t &problem_size, const size_t &individual_size);
        void display();
      private:
        std::vector<std::vector<int>> melody_;
        static std::default_random_engine generator_;
    };
} // namespace composer

#endif // COMPOSER_MELODY_H
