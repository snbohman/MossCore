#include <doctest/doctest.h>
#include <moss/moss.hpp>

using namespace moss;

namespace {

struct Counter { int ticks = 0; };

struct Config {
    Config(int w = 1, int h = 1) : width(w), height(h) { }
    int width;
    int height;
};

}

TEST_CASE("Resource") {
    entt::registry reg;

    GIVEN("A resource that does not exist") {
        auto res = cmd::Resource<Counter>::init(&reg);

        CHECK_FALSE(res.contains());
        CHECK(res.find() == nullptr);
    }

    GIVEN("emplace with defaults") {
        Counter& counter = cmd::Resource<Counter>::init(&reg).emplace();

        CHECK(counter.ticks == 0);
        CHECK(cmd::Resource<Counter>::init(&reg).contains());
    }

    GIVEN("emplace with constructor arguments") {
        Config& config = cmd::Resource<Config>::init(&reg).emplace(1280, 720);

        CHECK(config.width == 1280);
        CHECK(config.height == 720);
    }

    GIVEN("get returns the same object every time") {
        Counter& made = cmd::Resource<Counter>::init(&reg).emplace();
        Counter& got = cmd::Resource<Counter>::init(&reg).get();
        CHECK(&made == &got);

        got.ticks = 5;
        CHECK(cmd::Resource<Counter>::init(&reg).get().ticks == 5);   // a fresh command sees it
    }

    GIVEN("find returns a pointer to it") {
        cmd::Resource<Counter>::init(&reg).emplace().ticks = 3;

        Counter* found = cmd::Resource<Counter>::init(&reg).find();
        REQUIRE(found != nullptr);
        CHECK(found->ticks == 3);
    }

    GIVEN("Different types are independent") {
        cmd::Resource<Counter>::init(&reg).emplace();

        CHECK(cmd::Resource<Counter>::init(&reg).contains());
        CHECK_FALSE(cmd::Resource<Config>::init(&reg).contains());
    }

    GIVEN("erase") {
        cmd::Resource<Counter>::init(&reg).emplace();
        cmd::Resource<Counter>::init(&reg).erase();

        CHECK_FALSE(cmd::Resource<Counter>::init(&reg).contains());
    }

    GIVEN("A resource can be created again after erase") {
        cmd::Resource<Counter>::init(&reg).emplace().ticks = 9;
        cmd::Resource<Counter>::init(&reg).erase();

        CHECK(cmd::Resource<Counter>::init(&reg).emplace().ticks == 0);
    }
}
