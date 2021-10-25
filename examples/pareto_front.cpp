#include "matplot/matplot.h"
#include <chrono>
#include <iostream>
#include <map>
#include <pareto/archive.h>
#include <pareto/common/default_allocator.h>
#include <pareto/kd_tree.h>
#include <pareto/matplot/archive.h>
#include <random>

int main() {
    using std::cout;
    using std::endl;
    using namespace pareto;
    // Constructor
    size_t capacity = 1000;
    using allocator = std::allocator<std::pair<const ::pareto::point<double, 3>, unsigned>>;
    using Container = pareto::spatial_map<double, 3, unsigned, std::less<unsigned>, allocator>;
    archive<double, 3, unsigned, Container> ar(capacity, {min, max, min});



    // Element access
    ar(-2.57664, -1.52034, 0.600798) = 17;
    ar(-2.14255, -0.518684, -2.92346) = 32;
    ar(-1.63295, 0.912108, -2.12953) = 36;
    ar(-0.653036, 0.927688, -0.813932) = 13;
    ar(-0.508188, 0.871096, -2.25287) = 32;
    ar(-2.55905, -0.271349, 0.898137) = 6;
    ar(-2.31613, -0.219302, 0) = 8;

    std::cout << "Number of fronts: " << ar.size_fronts() << std::endl;

    std::cout << "Front Iterators:" << std::endl;
    for (auto it = ar.begin_front(); it != ar.end_front(); ++it) {
        std::cout << "Front with " << it->size() << " elements" << std::endl;
        for (const auto &[k, v] : *it) {
            std::cout << k << " -> " << v << std::endl;
        }
    }

    // Plot a single 2d archive
    matplot::hold(false);
    plot_archive(ar);
    matplot::save("archive2d.svg");
    matplot::show();
/*    ar(-0.639149, 1.89515, 0.858653) = 10;
    ar(-0.401531, 2.30172, 0.58125) = 39;
    ar(0.0728106, 1.91877, 0.399664) = 25;
    ar(-1.09756, 1.33135, 0.569513) = 20;
    ar(-0.894115, 1.01387, 0.462008) = 11;
    ar(-1.45049, 1.35763, 0.606019) = 17;
    ar(0.152711, 1.99514, -0.112665) = 13;
    ar(-2.3912, 0.395611, 2.78224) = 11;
    ar(-0.00292544, 1.29632, -0.578346) = 20;
    ar(0.157424, 2.30954, -1.23614) = 6;
    ar(0.453686, 1.02632, -2.24833) = 30;
    ar(0.693712, 1.12267, -1.37375) = 12;
    ar(1.49101, 3.24052, 0.724771) = 24;*/

    if (ar.contains({1.49101, 3.24052, 0.724771})) {
        std::cout << "Element access: " << ar(1.49101, 3.24052, 0.724771) << std::endl;
    } else {
        std::cout << "{1.49101, 3.24052, 0.724771} was dominated" << std::endl;
    }

    // Iterators
    std::cout << "Iterators:" << std::endl;
    for (const auto& [point, value]: ar) {
        cout << point << " -> " << value << endl;
    }

    std::cout << "Reversed Iterators:" << std::endl;
    for (auto it = ar.rbegin(); it != ar.rend(); ++it) {
        cout << it->first << " -> " << it->second << endl;
    }

    auto it = ar.rbegin();
    cout << it->first << " -> " << it->second << endl;

    return 0;
}
