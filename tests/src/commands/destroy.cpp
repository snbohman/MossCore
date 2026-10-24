#include <doctest/doctest.h>
#include <moss/moss.hpp>
#include <tst/simple.hpp>

using namespace moss;

TEST_CASE("Destroy") {
    tst::World w;

    GIVEN("One entity") {
        CHECK(cmd::Destroy::init(&w.reg).one(w.a));

        CHECK_FALSE(w.reg.valid(w.a));
        CHECK(cmd::Query<With<tst::Pos>>::init(&w.reg).count() == 2);   // a is gone from queries
    }

    GIVEN("An entity that is already gone") {
        auto destroy = cmd::Destroy::init(&w.reg);

        CHECK(destroy.one(w.a));
        CHECK_FALSE(destroy.one(w.a));   // skipped, not an error
    }

    GIVEN("Several entities") {
        DynamicView list{ w.a, w.b, w.c };

        CHECK(cmd::Destroy::init(&w.reg).many(list) == 3);
        CHECK(cmd::Query<With<tst::Pos>>::init(&w.reg).empty());
        CHECK(w.reg.valid(w.d));   // the rest is untouched
    }

    GIVEN("A list with entities that are already gone") {
        DynamicView list{ w.a, w.b };
        w.reg.destroy(w.a);

        CHECK(cmd::Destroy::init(&w.reg).many(list) == 1);
    }

    GIVEN("An empty list") {
        DynamicView none;
        CHECK(cmd::Destroy::init(&w.reg).many(none) == 0);
    }
}
