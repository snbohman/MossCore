#include <doctest.h>
#include <moss/meta/defines.hpp>
#include <moss/core/app.hpp>
#include <moss/core/mirror.hpp>
#include <moss/core/context.hpp>
#include <moss/ecs/system.hpp>
#include <moss/commands/commands.hpp>


using namespace moss;

struct Position : Component { float x; float y; };
struct PlayerTag : Component { };
struct EnemyTag : Component { };

class PlayerMovement : public System {
public:
    void build(const Key<key::WRITE>& key, const DynamicView& entities) override {
        commands::DynamicQuery<With<Position>> q;
        q.apply(key);

        auto [pos] = q.pool(entities);
        pos.x = 10; pos.y = 30;
    }

    void tick(const Key<key::READ>& key, const DynamicView& entities) override {
        commands::Query<With<Position>, commands::View< Include<Position>, Exclude<> >> q;
        q.apply(key); auto [pos] = q.pool();

        pos.x++; pos.y -= 2;
        if ((int)pos.x % 100 == 0) { M_INFOF("x100"); }
        if ((int)pos.y % 100 == 0) { M_INFOF("y100"); }
    }
};

class Player : public Context {
public:
    void init(Mirror& mirror) override {
        mirror.create()
            .attach<Position>()
            .attach<PlayerTag>()
            .connect<PlayerMovement>();
    }
};

TEST_CASE("Basic App") {
    App::instance()
        .init()
        .mount<Player>()
        .build()
        .run()
        .exit();
}
