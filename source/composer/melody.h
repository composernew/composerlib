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

        melody(smf::MidiEventList &notes);
        [[nodiscard]] const smf::MidiEventList &notes() const;
        void disp();
        void mutation(double mutation_strength);

      private:
        smf::MidiEventList notes_;
        static std::default_random_engine generator_;
    };

} // namespace composer

#endif // COMPOSER_MELODY_H
