#include <doctest/doctest.h>
#include <moss/moss.hpp>
#include <tst/simple.hpp>

using namespace moss;

TEST_CASE("Detach") {
    tst::World w;

    GIVEN("One component from one entity") {
        CHECK(cmd::Detach<With<tst::Pos>>::init(&w.reg).from(w.a) == 1);

        CHECK_FALSE(w.reg.all_of<tst::Pos>(w.a));
        CHECK(w.reg.all_of<tst::Vel>(w.a));   // other components stay
        CHECK(w.reg.valid(w.a));              // so does the entity
    }

    GIVEN("Several components, all present") {
        CHECK(cmd::Detach<With<tst::Pos, tst::Vel>>::init(&w.reg).from(w.a) == 2);
        CHECK_FALSE(w.reg.all_of<tst::Pos>(w.a));
        CHECK_FALSE(w.reg.all_of<tst::Vel>(w.a));
    }

    GIVEN("Several components, only some present") {
        // c has Pos but no Vel: only the one that exists is counted
        CHECK(cmd::Detach<With<tst::Pos, tst::Vel>>::init(&w.reg).from(w.c) == 1);
    }

    GIVEN("A component the entity does not have") {
        CHECK(cmd::Detach<With<tst::Pos>>::init(&w.reg).from(w.d) == 0);
        CHECK(cmd::Detach<With<tst::Pos>>::init(&w.reg).from(w.e) == 0);
    }

    GIVEN("A tag") {
        CHECK(cmd::Detach<With<tst::Flag>>::init(&w.reg).from(w.b) == 1);
        CHECK_FALSE(w.reg.all_of<tst::Flag>(w.b));
    }

    GIVEN("A list of entities") {
        DynamicView list{ w.a, w.b, w.c, w.d };

        // a, b, c have Pos; d does not
        CHECK(cmd::Detach<With<tst::Pos>>::init(&w.reg).from(list) == 3);
        CHECK(cmd::Query<With<tst::Pos>>::init(&w.reg).empty());
    }

    GIVEN("An entity that is already gone") {
        w.reg.destroy(w.a);
        CHECK(cmd::Detach<With<tst::Pos>>::init(&w.reg).from(w.a) == 0);
    }

    GIVEN("Detached components can be attached again") {
        cmd::Detach<With<tst::Pos>>::init(&w.reg).from(w.a);
        cmd::Attach<With<tst::Pos>>::init(&w.reg).to(w.a, 5, 5);

        CHECK(w.reg.get<tst::Pos>(w.a).x == 5);
    }
}
