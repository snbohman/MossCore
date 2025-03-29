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
    void build(Contex<contex::WRITE>& contex, const DynamicView& entity) override {
        Query<With<Position>> q;
        auto [pos] = q.pool(contex, entity);
        pos.x = 10; pos.y = 0;
    }

    void tick(Contex<contex::READ>& contex, const DynamicView& entity) override {
        Query<With<Position>> q;
        auto [pos] = q.pool(contex, entity);
        M_INFO("{}-{}", pos.x, pos.y);
        contex.quit();
    }
};


TEST_CASE("Basic App") {
    moss::App::init()
        .create()
            .attachComponent<Position>()
        /*    .attachComponent<PlayerTag>()*/
        /*    .attachSystem<Movement>()*/
        /*.create(10)*/
        /*    .attachComponent<Position>()*/
        /*    .attachComponent<EnemyTag>()*/
        .build()
        .run()
        .exit();
}
