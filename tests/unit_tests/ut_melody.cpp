#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <composer/melody.h>

TEST_CASE("Melody") {
    using namespace composer;
    melody m;
    m.notes({1,3,6,3});
    REQUIRE(m.notes().size() > 3);
}
