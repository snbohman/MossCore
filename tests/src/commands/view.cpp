#include <doctest.h>
#include <moss/moss.hpp>
#include <tst/components.hpp>
#include <tst/utils.hpp>

using namespace moss;


TEST_CASE("View") {
    entt::registry reg = tst::newRegistry();

    GIVEN("Including CompA") {
        auto view = cmd::View<
            Include<tst::CompA>
        >::init(&reg).view();

        CHECK(view->size() == 3);
        for (auto entity : view) {
            std::string_view uId = reg.get<tst::CompA>(entity).uId;
            tst::uIdCheck(uId, {"EntA", "EntD", "EntF"});
        }
    }

    GIVEN("Including CompA and CompB") {
        auto view = cmd::View<
            Include<tst::CompA, tst::CompB>
        >::init(&reg).view();

        CHECK(tst::vSize(view) == 1);
        for (auto entity : view) {
            std::string_view uIdA = reg.get<tst::CompA>(entity).uId;
            std::string_view uIdB = reg.get<tst::CompB>(entity).uId;
            tst::uIdCheck(uIdA, {"EntD"});
            tst::uIdCheck(uIdB, {"EntD"});
        }
    }

    GIVEN("Including CompA and Exlcuding CompB") {
        auto view = cmd::View<
            Include<tst::CompA>, Exclude<tst::CompB>
        >::init(&reg).view();

        CHECK(tst::vSize(view) == 2);
        for (auto entity : view) {
            std::string_view uId = reg.get<tst::CompA>(entity).uId;
            tst::uIdCheck(uId, {"EntA", "EntF"});
        }
    }
}
