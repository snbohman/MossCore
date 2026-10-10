#include "moss/commands/primitives.hpp"
#include <doctest/doctest.h>
#include <moss/moss.hpp>
#include <tst/components.hpp>
#include <tst/utils.hpp>

using namespace moss;


TEST_CASE("Query") {
    entt::registry reg = tst::newRegistry();

    GIVEN("Pools") {
        GIVEN("With CompA, Including CompA, Excluding CompB, CompC") {
            auto [comp] = cmd::Query<
                With<tst::CompA>,
                cmd::View<Include<tst::CompA>, Exclude<tst::CompB, tst::CompC>>
            >::init(&reg).atlas()[0];

            tst::uIdCheck(comp.uId, {"EntA"});
        }
    }

    GIVEN("Atlases") {
        GIVEN("With CompA, Including CompA") {
            Atlas<tst::CompA> atlas = cmd::Query<
                With<tst::CompA>,
                cmd::View<Include<tst::CompA>>
            >::init(&reg).atlas();

            CHECK(atlas.size() == 3);
            for (const auto& [comp] : atlas) {
                tst::uIdCheck(comp.uId, {"EntA", "EntD", "EntF"});
            }
        }

        GIVEN("With CompA, Including CompA, Excluding CompB") {
            Atlas<tst::CompA> atlas = cmd::Query<
                With<tst::CompA>,
                cmd::View<Include<tst::CompA>, Exclude<tst::CompB>>
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
                    cmd::View<Include<tst::CompA>, Exclude<tst::CompB, tst::CompC>>
                >::init(&reg).atlas()[0];

                comp.num = 10;
            }
            {
                auto [comp] = cmd::Query<
                    With<tst::CompA>,
                    cmd::View<Include<tst::CompA>, Exclude<tst::CompB, tst::CompC>>
                >::init(&reg).atlas()[0];

                CHECK(comp.num == 10);
            }
        }

        GIVEN("Atlas") {
            {
                Atlas<tst::CompA> atlas = cmd::Query<
                    With<tst::CompA>,
                    cmd::View<Include<tst::CompA>, Exclude<tst::CompB>>
                >::init(&reg).atlas();

                for (const auto& [comp] : atlas) {
                    comp.num = 10;
                }
            }
            {
                Atlas<tst::CompA> atlas = cmd::Query<
                    With<tst::CompA>,
                    cmd::View<Include<tst::CompA>, Exclude<tst::CompB>>
                >::init(&reg).atlas();

                for (const auto& [comp] : atlas) {
                    CHECK(comp.num == 10);
                }
            }
        }
    }
}
