#include <doctest.h>
#include <moss/meta/defines.hpp>
#include <moss/core/app.hpp>
#include <moss/core/mirror.hpp>
#include <moss/core/context.hpp>
#include <moss/ecs/system.hpp>
#include <moss/commands/primitives.hpp>
#include <moss/commands/query.hpp>


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
        pos.x = 10; pos.y = 0;
    }

    void tick(const Key<key::READ>& key, const DynamicView& entities) override {
        commands::DynamicQuery<With<Position>> q;
        q.apply(key);
        auto [pos] = q.pool(entities);

        M_INFO("POS: {}-{}", pos.x, pos.y);
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
    App app;
    app.init()
        .mount<Player>()
        .build()
        .run()
        .exit();
}
