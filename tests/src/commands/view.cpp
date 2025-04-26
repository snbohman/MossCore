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
            CHECK(uId != "EntB"); CHECK(uId != "EntC"); CHECK(uId != "EntE");
        }
    }

    
    GIVEN("Including CompB") {
        auto view = cmd::View<
            Include<tst::CompB>
        >::init(&reg).view();

        CHECK(view->size() == 3);
        for (auto entity : view) {
            std::string_view uId = reg.get<tst::CompB>(entity).uId;
            CHECK(uId != "EntA"); CHECK(uId != "EntC"); CHECK(uId != "EntF");
        }
    }

    GIVEN("Including CompC") {
        auto view = cmd::View<
            Include<tst::CompC>
        >::init(&reg).view();

        CHECK(view->size() == 3);
        for (auto entity : view) {
            std::string_view uId = reg.get<tst::CompC>(entity).uId;
            CHECK(uId != "EntA"); CHECK(uId != "EntB"); CHECK(uId != "EntD");
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
            CHECK(uIdA == "EntD"); CHECK(uIdB == "EntD");
        }
    }

    GIVEN("Including CompA and Exlcuding CompB") {
        auto view = cmd::View<
            Include<tst::CompA>, Exclude<tst::CompB>
        >::init(&reg).view();

        CHECK(tst::vSize(view) == 2);
        for (auto entity : view) {
            std::string_view uId = reg.get<tst::CompA>(entity).uId;
            CHECK(uId != "EntB"); CHECK(uId != "EntC");
            CHECK(uId != "EntD"); CHECK(uId != "EntE");
        }
    }
}
