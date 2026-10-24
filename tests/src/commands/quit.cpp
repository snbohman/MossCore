#include <doctest/doctest.h>
#include <moss/moss.hpp>

using namespace moss;

TEST_CASE("Quit") {
    entt::registry reg;
    bool flag = false;

    GIVEN("Nothing requested yet") {
        CHECK_FALSE(cmd::Quit::init(&reg, &flag).requested());
        CHECK_FALSE(flag);
    }

    GIVEN("quit sets the flag") {
        cmd::Quit::init(&reg, &flag).quit();

        CHECK(flag);
        CHECK(cmd::Quit::init(&reg, &flag).requested());
    }

    GIVEN("Quitting twice is harmless") {
        auto quit = cmd::Quit::init(&reg, &flag);
        quit.quit();
        quit.quit();

        CHECK(flag);
    }
}
