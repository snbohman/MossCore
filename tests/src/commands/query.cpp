#include <doctest/doctest.h>
#include <moss/moss.hpp>
#include <tst/components.hpp>
#include <tst/simple.hpp>
#include <tst/utils.hpp>

using namespace moss;

/* ------------------- simple cases ------------------- */

TEST_CASE("Query: fixture registry") {
    entt::registry reg = tst::newRegistry();

    GIVEN("Pools") {
        GIVEN("With CompA, Excluding CompB, CompC") {
            auto [comp] = cmd::Query<
                With<tst::CompA>,
                cmd::View<Include<>, Exclude<tst::CompB, tst::CompC>>
            >::init(&reg).atlas()[0];

            tst::uIdCheck(comp.uId, {"EntA"});
        }

        GIVEN("The old redundant form still works") {
            auto [comp] = cmd::Query<
                With<tst::CompA>,
                cmd::View<Include<tst::CompA>, Exclude<tst::CompB, tst::CompC>>
            >::init(&reg).atlas()[0];

            tst::uIdCheck(comp.uId, {"EntA"});
        }
    }

    GIVEN("Atlases") {
        GIVEN("With CompA") {
            Atlas<tst::CompA> atlas = cmd::Query<With<tst::CompA>>::init(&reg).atlas();

            CHECK(atlas.size() == 3);
            for (const auto& [comp] : atlas) {
                tst::uIdCheck(comp.uId, {"EntA", "EntD", "EntF"});
            }
        }

        GIVEN("With CompA, Excluding CompB") {
            Atlas<tst::CompA> atlas = cmd::Query<
                With<tst::CompA>,
                cmd::View<Include<>, Exclude<tst::CompB>>
            >::init(&reg).atlas();

            CHECK(atlas.size() == 2);
            for (const auto& [comp] : atlas) {
                tst::uIdCheck(comp.uId, {"EntA", "EntF"});
            }
        }
    }

    GIVEN("Mutability") {
        GIVEN("Pool") {
            {
                auto [comp] = cmd::Query<
                    With<tst::CompA>,
                    cmd::View<Include<>, Exclude<tst::CompB, tst::CompC>>
                >::init(&reg).atlas()[0];

                comp.num = 10;
            }
            {
                auto [comp] = cmd::Query<
                    With<tst::CompA>,
                    cmd::View<Include<>, Exclude<tst::CompB, tst::CompC>>
                >::init(&reg).atlas()[0];

                CHECK(comp.num == 10);
            }
        }

        GIVEN("Atlas") {
            {
                Atlas<tst::CompA> atlas = cmd::Query<
                    With<tst::CompA>,
                    cmd::View<Include<>, Exclude<tst::CompB>>
                >::init(&reg).atlas();

                for (const auto& [comp] : atlas) {
                    comp.num = 10;
                }
            }
            {
                Atlas<tst::CompA> atlas = cmd::Query<
                    With<tst::CompA>,
                    cmd::View<Include<>, Exclude<tst::CompB>>
                >::init(&reg).atlas();

                for (const auto& [comp] : atlas) {
                    CHECK(comp.num == 10);
                }
            }
        }
    }
}

/* ------------------------------ (new moss) cases ------------------------------ */

TEST_CASE("Query: matching") {
    tst::World w;

    GIVEN("With only") {
        auto q = cmd::Query<With<tst::Pos>>::init(&w.reg);
        CHECK(q.count() == 3);
        CHECK(tst::sorted(q.entities()) == tst::set(w.a, w.b, w.c));
        CHECK_FALSE(q.empty());
    }

    GIVEN("With two components") {
        auto q = cmd::Query<With<tst::Pos, tst::Vel>>::init(&w.reg);
        CHECK(tst::sorted(q.entities()) == tst::set(w.a, w.b));
    }

    GIVEN("A tag in Include") {
        auto q = cmd::Query<With<tst::Pos>, cmd::View<Include<tst::Flag>>>::init(&w.reg);
        CHECK(tst::sorted(q.entities()) == tst::set(w.b, w.c));
    }

    GIVEN("Exclude only") {
        auto q = cmd::Query<With<tst::Pos>, cmd::View<Include<>, Exclude<tst::Vel>>>::init(&w.reg);
        CHECK(tst::sorted(q.entities()) == tst::set(w.c));
    }

    GIVEN("Include and Exclude together") {
        auto q = cmd::Query<
            With<tst::Pos>,
            cmd::View<Include<tst::Flag>, Exclude<tst::Vel>>
        >::init(&w.reg);
        CHECK(tst::sorted(q.entities()) == tst::set(w.c));
    }

    GIVEN("Redundant Include of a With type") {
        auto q = cmd::Query<With<tst::Pos>, cmd::View<Include<tst::Pos>>>::init(&w.reg);
        CHECK(q.count() == 3);
    }

    GIVEN("A query nothing matches") {
        auto q = cmd::Query<
            With<tst::Vel>,
            cmd::View<Include<tst::Flag>, Exclude<tst::Pos>>
        >::init(&w.reg);
        CHECK(q.empty());
        CHECK(q.count() == 0);
        CHECK(q.atlas().empty());
        CHECK_FALSE(q.first().has_value());
    }

    GIVEN("A non-trivial component") {
        auto rows = cmd::Query<With<tst::Name>>::init(&w.reg).atlas();
        REQUIRE(rows.size() == 1);
        CHECK(std::get<0>(rows[0]).text == "solo");
    }

    GIVEN("The raw view") {
        auto q = cmd::Query<With<tst::Pos>>::init(&w.reg);
        CHECK(q.view().contains(w.a));
        CHECK_FALSE(q.view().contains(w.d));
    }
}

TEST_CASE("Query: subsets keep their order") {
    tst::World w;
    auto q = cmd::Query<With<tst::Pos>>::init(&w.reg);

    GIVEN("A subset with a non-matching entity") {
        DynamicView order{ w.c, w.d, w.a };   // d has no Pos

        auto rows = q.atlas(order);
        REQUIRE(rows.size() == 2);
        CHECK(std::get<0>(rows[0]).x == 3);
        CHECK(std::get<0>(rows[1]).x == 1);

        DynamicView expected{ w.c, w.a };
        CHECK(q.entities(order) == expected);
    }

    GIVEN("A dead entity in the subset") {
        DynamicView order{ w.a, w.b };
        w.reg.destroy(w.a);

        auto rows = q.atlas(order);
        REQUIRE(rows.size() == 1);
        CHECK(std::get<0>(rows[0]).x == 2);
    }

    GIVEN("An empty subset") {
        DynamicView none;
        CHECK(q.atlas(none).empty());
        CHECK(q.entities(none).empty());
    }
}

TEST_CASE("Query: each") {
    tst::World w;

    GIVEN("Visiting every match") {
        int sum = 0;
        usize visited = 0;

        cmd::Query<With<tst::Pos, tst::Vel>>::init(&w.reg).each(
            [&](Entity, tst::Pos& pos, tst::Vel& vel) {
                sum += pos.x + vel.dx;
                visited++;
            }
        );

        CHECK(visited == 2);
        CHECK(sum == (1 + 10) + (2 + 20));
    }

    GIVEN("Writing through each") {
        cmd::Query<With<tst::Pos>>::init(&w.reg).each(
            [](Entity, tst::Pos& pos) { pos.x = 100; }
        );

        CHECK(w.reg.get<tst::Pos>(w.a).x == 100);
        CHECK(w.reg.get<tst::Pos>(w.c).x == 100);
    }

    GIVEN("Each over a subset") {
        DynamicView subset{ w.b, w.d, w.c };   // d has no Pos
        std::vector<int> seen;

        cmd::Query<With<tst::Pos>>::init(&w.reg).each(
            subset, [&](Entity, tst::Pos& pos) { seen.push_back(pos.x); }
        );

        std::vector<int> expected{ 2, 3 };
        CHECK(seen == expected);
    }
}

TEST_CASE("Query: one entity") {
    tst::World w;

    GIVEN("find") {
        auto q = cmd::Query<With<tst::Pos, tst::Vel>>::init(&w.reg);

        auto hit = q.find(w.a);
        REQUIRE(hit.has_value());
        CHECK(std::get<1>(*hit).dx == 10);

        CHECK_FALSE(q.find(w.c).has_value());   // Pos but no Vel
    }

    GIVEN("get") {
        auto [pos] = cmd::Query<With<tst::Pos>>::init(&w.reg).get(w.b);
        CHECK(pos.x == 2);

        pos.x = 50;
        CHECK(w.reg.get<tst::Pos>(w.b).x == 50);
    }

    GIVEN("copy of a single component is optional<T>") {
        auto q = cmd::Query<With<tst::Pos>>::init(&w.reg);

        auto copy = q.copy(w.a);
        REQUIRE(copy.has_value());
        CHECK(copy->x == 1);

        copy->x = 99;   // a copy: the registry is untouched
        CHECK(w.reg.get<tst::Pos>(w.a).x == 1);

        CHECK_FALSE(q.copy(w.d).has_value());
    }

    GIVEN("copy of several components is optional<tuple>") {
        auto q = cmd::Query<With<tst::Pos, tst::Vel>>::init(&w.reg);

        auto copy = q.copy(w.b);
        REQUIRE(copy.has_value());
        CHECK(std::get<0>(*copy).x == 2);
        CHECK(std::get<1>(*copy).dy == 20);
    }

    GIVEN("first") {
        auto q = cmd::Query<With<tst::Pos>>::init(&w.reg);
        CHECK(q.first().has_value());
        CHECK_FALSE(cmd::Query<With<tst::Ctor>>::init(&w.reg).first().has_value());
    }

    GIVEN("single") {
        auto [pos] = cmd::Query<
            With<tst::Pos>,
            cmd::View<Include<tst::Flag>, Exclude<tst::Vel>>
        >::init(&w.reg).single();
        CHECK(pos.x == 3);
    }

    GIVEN("contains") {
        auto q = cmd::Query<With<tst::Pos>>::init(&w.reg);
        CHECK(q.contains(w.a));
        CHECK_FALSE(q.contains(w.d));

        w.reg.destroy(w.a);
        CHECK_FALSE(q.contains(w.a));
    }

    GIVEN("has works for any component, tags included") {
        auto q = cmd::Query<With<tst::Pos>>::init(&w.reg);
        CHECK(q.has<tst::Flag>(w.b));
        CHECK_FALSE(q.has<tst::Flag>(w.a));
        CHECK(q.has<tst::Pos, tst::Vel>(w.a));
        CHECK_FALSE(q.has<tst::Pos, tst::Vel>(w.c));
        CHECK(q.has<tst::Name>(w.e));   // not in the query, still checkable
    }
}

TEST_CASE("Query: mutability") {
    tst::World w;

    GIVEN("An atlas is writable") {
        {
            auto atlas = cmd::Query<With<tst::Pos>>::init(&w.reg).atlas();
            for (auto& [pos] : atlas) pos.x += 100;
        }

        CHECK(w.reg.get<tst::Pos>(w.a).x == 101);
        CHECK(w.reg.get<tst::Pos>(w.b).x == 102);
        CHECK(w.reg.get<tst::Pos>(w.c).x == 103);
        CHECK(w.reg.get<tst::Vel>(w.a).dx == 10);   // other components untouched
    }
}

TEST_CASE("Query: handle") {
    GIVEN("A default-constructed query is unbound") {
        cmd::Query<With<tst::Pos>> q;
        CHECK_FALSE(q.bound());
    }

    GIVEN("apply binds it later") {
        tst::World w;

        cmd::Query<With<tst::Pos>> q;
        q.apply(&w.reg);

        CHECK(q.bound());
        CHECK(q.count() == 3);
    }
}
