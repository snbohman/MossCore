#include <doctest/doctest.h>
#include <moss/moss.hpp>
#include <tst/components.hpp>
#include <tst/simple.hpp>
#include <tst/utils.hpp>

using namespace moss;

TEST_CASE("Attach: fixture registry") {
    entt::registry reg = tst::newRegistry();

    GIVEN("Attaching CompD") {
        auto e = reg.create();
        auto [comp] = cmd::Attach<With<tst::CompD>>::init(&reg).to(e);
        comp.num = 9;

        CHECK(reg.get<tst::CompD>(e).num == 9);
    }
}

TEST_CASE("Attach: one entity") {
    tst::World w;
    Entity e = w.reg.create();

    GIVEN("A default-constructed component") {
        auto [pos] = cmd::Attach<With<tst::Pos>>::init(&w.reg).to(e);
        CHECK(pos.x == 0);
        CHECK(pos.y == 0);

        pos.x = 7;   // the returned pool refers to the stored component
        CHECK(w.reg.get<tst::Pos>(e).x == 7);
    }

    GIVEN("Positional arguments on an aggregate") {
        cmd::Attach<With<tst::Pos>>::init(&w.reg).to(e, 4, 5);

        CHECK(w.reg.get<tst::Pos>(e).x == 4);
        CHECK(w.reg.get<tst::Pos>(e).y == 5);
    }

    GIVEN("A component with a constructor") {
        auto [ctor] = cmd::Attach<With<tst::Ctor>>::init(&w.reg).to(e, 2, 3);
        CHECK(ctor.sum == 5);
    }

    GIVEN("A non-trivial argument") {
        cmd::Attach<With<tst::Name>>::init(&w.reg).to(e, std::string("hello"));
        CHECK(w.reg.get<tst::Name>(e).text == "hello");
    }

    GIVEN("Several types at once") {
        auto [pos, vel] = cmd::Attach<With<tst::Pos, tst::Vel>>::init(&w.reg).to(e);

        pos.x = 1;
        vel.dx = 2;
        CHECK(w.reg.get<tst::Pos>(e).x == 1);
        CHECK(w.reg.get<tst::Vel>(e).dx == 2);
    }

    GIVEN("A tag") {
        cmd::Attach<With<tst::Flag>>::init(&w.reg).to(e);
        CHECK(w.reg.all_of<tst::Flag>(e));
    }

    GIVEN("A tag mixed with data") {
        cmd::Attach<With<tst::Pos, tst::Flag>>::init(&w.reg).to(e);

        CHECK(w.reg.all_of<tst::Pos>(e));
        CHECK(w.reg.all_of<tst::Flag>(e));
    }

    GIVEN("A component the entity already has is replaced") {
        cmd::Attach<With<tst::Pos>>::init(&w.reg).to(w.a, 8, 9);   // logs a warning

        CHECK(w.reg.get<tst::Pos>(w.a).x == 8);
        CHECK(w.reg.get<tst::Pos>(w.a).y == 9);
        CHECK(w.reg.get<tst::Vel>(w.a).dx == 10);   // the rest is untouched
        CHECK(cmd::Query<With<tst::Pos>>::init(&w.reg).count() == 3);
    }
}

TEST_CASE("Attach: a list of entities") {
    tst::World w;

    GIVEN("Defaults on each") {
        DynamicView list{ w.d, w.e };
        auto atlas = cmd::Attach<With<tst::Pos>>::init(&w.reg).to(list);

        CHECK(atlas.size() == 2);
        CHECK(w.reg.all_of<tst::Pos>(w.d));
        CHECK(w.reg.all_of<tst::Pos>(w.e));
    }

    GIVEN("The same arguments on each") {
        DynamicView list{ w.c, w.d, w.e };
        auto atlas = cmd::Attach<With<tst::Vel>>::init(&w.reg).to(list, 7, 8);
        // c and e are new; d already has a Vel and is replaced

        REQUIRE(atlas.size() == 3);
        for (auto& [vel] : atlas) {
            CHECK(vel.dx == 7);
            CHECK(vel.dy == 8);
        }
    }

    GIVEN("A tag on each") {
        DynamicView list{ w.a, w.d };
        cmd::Attach<With<tst::Flag>>::init(&w.reg).to(list);

        CHECK(w.reg.all_of<tst::Flag>(w.a));
        CHECK(w.reg.all_of<tst::Flag>(w.d));
    }

    GIVEN("An empty list") {
        DynamicView none;
        auto atlas = cmd::Attach<With<tst::Pos>>::init(&w.reg).to(none);
        CHECK(atlas.empty());
    }

    GIVEN("The result of a Query as the target") {
        auto lacking = cmd::Query<
            With<tst::Pos>,
            cmd::View<Include<>, Exclude<tst::Vel>>
        >::init(&w.reg).entities();   // only c

        cmd::Attach<With<tst::Vel>>::init(&w.reg).to(lacking, 1, 2);

        CHECK(w.reg.get<tst::Vel>(w.c).dx == 1);
        CHECK(cmd::Query<With<tst::Vel>>::init(&w.reg).count() == 4);
    }
}
