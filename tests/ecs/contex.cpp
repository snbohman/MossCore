#include <doctest.h>
#include <moss/core/app.hpp>
#include <moss/core/contex.hpp>
#include <moss/core/ecs.hpp>
#include <moss/meta/defines.hpp>


struct Position : Component { float x; float y; };
struct PlayerTag : Component { };
struct EnemyTag : Component { };

using namespace moss;

class Movement : public moss::System {
    void build(sys::BuildCrate crate) override {
        crate.fluent;
        create.contex;

        Query<With<Position>> q;
        auto [pos] = q.pool(contex, entity);
        pos.x = 10; pos.y = 0;
    }

    void tick(sys::BuildCrate crate) override {
        Query<With<Position>> q;
        auto [pos] = q.pool(contex, entity);
        M_INFO("{}-{}", pos.x, pos.y);
        contex.quit();
    }
};


TEST_CASE("Basic App") {
    App app; app.init();

    Contex<ctx::READ> contex;
    app.load(contex);

    cmd/command::Create<10> c;
    cmd::DynamicView v1 = c.build(contex).view();
    cmd::DynamicView v2 = c.build(contex).view();

    /*
    alternatively:

    cmd::Create<10> c(contex);       // always build c with contex
    cmd::DynamicView v1 = c.view();
    cmd::DynamicView v2 = c.view();

    */

    cmd::Attach<Movement, Position, PlayerTag> ap;
    cmd::Attach<Position, EnemyTag> ae;
    ap.build(contex, v1);
    ae.build(contex, v2);

    app.build(contex);
    app.run();
    app.clean();

    Fluent<flt::READ> fluent;
    fluent
        .create(1)
            .attach<Movement, Position, PlayerTag>()
        .create(10)
            .attach<Position, EnemyTag>()
        .attach<WorldWide>(fluent.view<Position>());

    app.build(fluent);
    app.run();
    app.clean();

    app.exit();
}
