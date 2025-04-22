#include <doctest.h>
#include <moss/moss.hpp>


using namespace moss;

struct Position : Component { float x; float y; };
struct PlayerTag : Component { };
struct EnemyTag : Component { };

class PlayerMovement : public System {
public:
    void build(const Key<key::WRITE>& key, const DynamicView& entities) override {
        { // First option
            auto [pos] = cmd::DynamicQuery<With<Position>>::init(key).pool(entities);
            pos.x = 10; pos.y = 30;
        }
        { // Second option
            auto q = cmd::DynamicQuery<With<Position>>::init(key);
            auto [pos] = q.pool(entities);
            pos.x = 10; pos.y = 30;
        }
        { // Thrid option
            cmd::DynamicQuery<With<Position>> q(key);
            auto [pos] = q.pool(entities);
            pos.x = 10; pos.y = 30;
        }
        { // Fourth option
            cmd::DynamicQuery<With<Position>> q;
            q.apply(key);
            auto [pos] = q.pool(entities);
            pos.x = 10; pos.y = 30;
        }
    }

    void tick(const Key<key::READ>& key, const DynamicView& entities) override {
        /* Usage can either be DynamicQuery, like in build, or now a usage of Query */
        auto [pos] = cmd::Query<
            With<Position>,
            cmd::View<Include<PlayerTag>>
        >::init(key).pool();

        pos.x++; pos.y -= 2;
        CHECK(pos.x == 11);
        CHECK(pos.y == 28);

        cmd::Quit::init(key).quit();
    }
};

class Player : public Context {
public:
    void init(Mirror& mirror) override {
        mirror.create()
            .attach<Position, PlayerTag>()
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
