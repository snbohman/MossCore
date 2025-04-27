#include <doctest.h>
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
            >::init(&reg).pool();

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
            for (const auto& pool : atlas) {
                auto [comp] = pool;
                tst::uIdCheck(comp.uId, {"EntA", "EntD", "EntF"});
            }
        }

        GIVEN("With CompA, Including CompA, Excluding CompB") {
            Atlas<tst::CompA> atlas = cmd::Query<
                With<tst::CompA>,
                cmd::View<Include<tst::CompA>, Exclude<tst::CompB>>
            >::init(&reg).atlas();

            CHECK(atlas.size() == 2);
            for (const auto& pool : atlas) {
                auto [comp] = pool;
                tst::uIdCheck(comp.uId, {"EntA", "EntF"});
            }
        }
    }

}
