/*
tests/read.cpp

Testing View and Query structs only utelizing
entt::registies and not contex.

*/

#include <doctest.h>
#include <moss/core/ecs/read.hpp>

//////////////////////////
//// -- Base World -- ////
//////////////////////////
#define PLAYER_ENEMY_REGISTRY \
    entt::registry registry; \
    auto e1 = registry.create(); \
    registry.emplace<Position>(e1, 100.f, 50.f); \
    registry.emplace<Health>(e1, 99.f, 80.f); \
    registry.emplace<PlayerTag>(e1); \
    auto e2 = registry.create(); \
    registry.emplace<Position>(e2, -100.f, -50.f); \
    registry.emplace<EnemyTag>(e2); \


struct Position { float x; float y; };
struct Health { float water; float hunger; };
struct PlayerTag { };
struct EnemyTag { };


//////////////////////////
//// -- Unit Tests -- ////
//////////////////////////
TEST_CASE("View") {
    PLAYER_ENEMY_REGISTRY;

    {   // Enemy
        moss::View<moss::Include<Position>, moss::Exclude<Health>> view;
        auto v = view.apply(registry);
        CHECK(std::distance(v.begin(), v.end()) == 1);
    }

    {   // Player
        moss::View<moss::Include<Position>, moss::Exclude<EnemyTag>> view;
        auto v = view.apply(registry);
        CHECK(std::distance(v.begin(), v.end()) == 1);
    }

    {   // Enemy and Player
        moss::View<moss::Include<Position>, moss::Exclude<>> view;
        auto v = view.apply(registry);
        CHECK(std::distance(v.begin(), v.end()) == 2);
    }

    {   // Player
        moss::View<moss::Include<Position, Health>, moss::Exclude<>> view;
        auto v = view.apply(registry);
        CHECK(std::distance(v.begin(), v.end()) == 1);
    }
}

TEST_CASE("Queries") {
    PLAYER_ENEMY_REGISTRY;

    CHECK_NOTHROW(
        moss::Query<moss::With<Position>, moss::View< moss::Include<PlayerTag>, moss::Exclude<> >>()
    );

    CHECK_NOTHROW(
        moss::Query<moss::With<Position, Health>, moss::View< moss::Include<PlayerTag>, moss::Exclude<> >>()
    );

    CHECK_NOTHROW(
        moss::Query<moss::With<Position>, moss::View< moss::Include<EnemyTag>, moss::Exclude<> >>()
    );

    CHECK_THROWS(   // No View::Include
        moss::Query<moss::With<Position>, moss::View< moss::Include<>, moss::Exclude<> >>()
    );
}


TEST_CASE("Pools") {
    PLAYER_ENEMY_REGISTRY;

    {   // Player
        moss::Query<moss::With<Position>, moss::View< moss::Include<PlayerTag>, moss::Exclude<> >> q;
        auto [pos] = q.pool(registry);
        CHECK(pos.x == 100); CHECK(pos.y == 50);
    }

    {   // Player
        moss::Query<moss::With<Position, Health>, moss::View< moss::Include<PlayerTag>, moss::Exclude<> >> q;
        auto [pos, health] = q.pool(registry);
        CHECK(pos.x == 100); CHECK(pos.y == 50);
        CHECK(health.water == 99); CHECK(health.hunger == 80);
    }

    {   // Enemy
        moss::Query<moss::With<Position>, moss::View< moss::Include<EnemyTag>, moss::Exclude<> >> q;
        auto [pos] = q.pool(registry);
        CHECK(pos.x == -100); CHECK(pos.y == -50);
    }
}

TEST_CASE("Atlases") {
    PLAYER_ENEMY_REGISTRY;

    {   // Player and Enemy
        moss::Query<moss::With<Position>, moss::View< moss::Include<Position>, moss::Exclude<> >> q;
        auto atlas = q.atlas(registry);
        CHECK(atlas.size() == 2);
    }

    {   // Player (even why?)
        moss::Query<moss::With<Position, Health>, moss::View< moss::Include<Health>, moss::Exclude<> >> q;
        auto atlas = q.atlas(registry);
        CHECK(atlas.size() == 1);
    }
}
