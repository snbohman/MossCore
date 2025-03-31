#include <doctest.h>
#include <moss/meta/defines.hpp>
#include <moss/core/app.hpp>
#include <moss/core/fluent.hpp>
#include <moss/ecs/system.hpp>
#include <moss/commands/primitives.hpp>
#include <moss/commands/query.hpp>

using namespace moss;

struct Position : Component { float x; float y; };
struct PlayerTag : Component { };
struct EnemyTag : Component { };

class Movement : public System {
    void build(Contex<contex::WRITE>& contex, Entity entity) override {
        commands::write::DynamicQuery<With<Position>> q;
        q.apply(contex);

        auto [pos] = q.pool({ entity });
        pos.x = 10; pos.y = 0;
    }

    void tick(Contex<contex::READ>& contex, Entity entity) override {
        commands::write::DynamicQuery<With<Position>> q;
        q.apply(contex);
        auto [pos] = q.pool({ entity });

        M_INFO("POS: {}-{}", pos.x, pos.y);
    }
};


TEST_CASE("Basic App") {
    App app;
    app.init();

    Contex<contex::READ> contex;
    app.loadContex(contex);

    Create<10> c;
    DynamicView v = c.apply(contex);
}
