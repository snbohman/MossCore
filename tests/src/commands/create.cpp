#include <doctest/doctest.h>
#include <moss/moss.hpp>

using namespace moss;

TEST_CASE("Create") {
    entt::registry reg;

    GIVEN("One entity") {
        Entity e = cmd::Create::init(&reg).one();

        CHECK(reg.valid(e));
        CHECK(reg.orphan(e));   // no components yet
    }

    GIVEN("Several entities") {
        DynamicView list = cmd::Create::init(&reg).many(5);

        REQUIRE(list.size() == 5);
        for (Entity e : list) CHECK(reg.valid(e));

        std::sort(list.begin(), list.end());
        CHECK(std::adjacent_find(list.begin(), list.end()) == list.end());   // all distinct
    }

    GIVEN("Zero entities is allowed") {
        CHECK(cmd::Create::init(&reg).many(0).empty());
    }

    GIVEN("Separate calls give separate entities") {
        auto create = cmd::Create::init(&reg);
        CHECK(create.one() != create.one());
    }
}
