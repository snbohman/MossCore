#include <doctest.h>
#include <moss/moss.hpp>

#include <components.hpp>
#include <systems/systems.hpp>


namespace tst {

class Contex : moss::Context {
    void init(moss::Mirror& mirror) override {
        mirror
            .create(1).attach<CompA, CompB, CompC>()
            .create(2).attach<CompA, CompB>()
            .create(3).attach<CompA, CompC>()
            .create(4).attach<CompA>()
            .connect<SysAttach, SysCreate, SysQuery, SysView>();
    }
};

}


SCENARIO("Main") {
    GIVEN("A new app") {
        moss::App::instance()
            .init()
            .mount<tst::Contex>()
            .build()
            .run()
            .exit();
    }
}
