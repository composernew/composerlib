//
// Created by Alan Freitas on 12/14/20.
//

#include "melody.h"

namespace composer {
    const std::vector<int> &melody::notes() const { return notes_; }
    void melody::notes(const std::vector<int> &notes) { notes_ = notes; }
}

