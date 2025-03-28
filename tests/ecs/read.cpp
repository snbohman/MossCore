#include <catch_amalgamated.hpp>
#include <moss/core/ecs/read.hpp>


struct Position { float x; float y; };
struct Health { float water; float hunger; };
struct PlayerTag { };
struct EnemyTag { };

TEST_CASE("Entity view") {
    entt::registry registry;

    auto e1 = registry.create();
    registry.emplace<Position>(e1, 100, 50);
    registry.emplace<Health>(e1, 99, 80);
    registry.emplace<PlayerTag>(e1, 99, 80);

    auto e2 = registry.create();
    registry.emplace<Position>(e2, -100, -50);
    registry.emplace<EnemyTag>(e2, 51, 27);

    moss::View<moss::Include<Position>, moss::Exclude<Health>> view1;
    auto v1 = view1.apply(registry); // enemy

    moss::View<moss::Include<Position>, moss::Exclude<EnemyTag>> view2;
    auto v2 = view2.apply(registry); // player

    moss::View<moss::Include<Position>, moss::Exclude<>> view3;
    auto v3 = view3.apply(registry); // enemy and player

    moss::View<moss::Include<Position, Health>, moss::Exclude<>> view4;
    auto v4 = view4.apply(registry); // player


    REQUIRE(std::distance(v1.begin(), v1.end()) == 1);
    REQUIRE(registry.get<Position>(*v1.begin()).x == -100);

    REQUIRE(std::distance(v2.begin(), v2.end()) == 1);
    REQUIRE(registry.get<Position>(*v1.begin()).x == 100);

    REQUIRE(std::distance(v3.begin(), v3.end()) == 2);

    REQUIRE(std::distance(v4.begin(), v4.end()) == 1);
}

TEST_CASE("Entity query, pool and atlas") {
    entt::registry registry;

    auto e1 = registry.create();
    registry.emplace<Position>(e1, 100, 50);
    registry.emplace<Health>(e1, 99, 80);
    registry.emplace<PlayerTag>(e1);

    auto e2 = registry.create();
    registry.emplace<Position>(e2, -100, -50);
    registry.emplace<EnemyTag>(e2);


    // player
    moss::Query<
        moss::With<Position>,
        moss::View<
            moss::Include<PlayerTag>,
            moss::Exclude<>
        >
    > q1; auto [p1] = q1.pool(registry);

    // player
    moss::Query<
        moss::With<Position, Health>,
        moss::View<
            moss::Include<PlayerTag>,
            moss::Exclude<>
        >
    > q2; auto [p2, h1] = q2.pool(registry);

    // enemy
    moss::Query<
        moss::With<Position>,
        moss::View<
            moss::Include<EnemyTag>,
            moss::Exclude<>
        >
    > q3; auto [p3] = q3.pool(registry);


    REQUIRE(p1.x == 100);
    REQUIRE(p2.x == -100);
    REQUIRE(h1.water == 99);
    REQUIRE(p3.x == -100);
}
