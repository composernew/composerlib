//
// Created by Alan Freitas on 12/14/20.
//

#ifndef COMPOSER_MELODY_H
#define COMPOSER_MELODY_H

#include <vector>

namespace composer {
    class melody {
      public:
        [[nodiscard]] const std::vector<int> &notes() const;
        void notes(const std::vector<int> &notes);

      private:
        std::vector<int> notes_;
    };

} // namespace composer

#endif // COMPOSER_MELODY_H
