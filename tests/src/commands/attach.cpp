#include <doctest/doctest.h>
#include <moss/moss.hpp>
#include <tst/components.hpp>
#include <tst/utils.hpp>

using namespace moss;


TEST_CASE("Attach") {
    entt::registry reg = tst::newRegistry();

    GIVEN("Dynamic Attach") {
        GIVEN("Attaching CompA") {
            auto e = reg.create();
            auto [comp] = cmd::DynamicAttach<With<tst::CompD>>::init(&reg).pool({e});
            comp.num = 9;

            CHECK(reg.get<tst::CompD>(e).num == 9);
            reg.destroy(e);
        }

        GIVEN("Attaching CompA, CompB") { }
        GIVEN("Attaching CompA, onto CompA") { }

    }
}
