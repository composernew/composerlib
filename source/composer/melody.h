//
// Created by Alan Freitas on 12/14/20.
//

#ifndef COMPOSER_MELODY_H
#define COMPOSER_MELODY_H

#include <vector>
#include <MidiFile.h>
#include <random>

namespace composer {
    class melody {
      public:

        melody(std::vector<std::vector<int>> &notes);
        [[nodiscard]] const std::vector<std::vector<int>> &notes() const;
        void disp();
        void mutation(double mutation_strength);

      private:
        std::vector<std::vector<int>> melody_;
        static std::default_random_engine generator_;
    };

} // namespace composer

#endif // COMPOSER_MELODY_H
