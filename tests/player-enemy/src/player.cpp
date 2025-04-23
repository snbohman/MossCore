#include <doctest.h>
#include <player.hpp>
#include <components.hpp>


void PlayerMovement::build(const moss::Key<moss::key::WRITE>& key, const moss::DynamicView& entities) {
    auto [pos] = moss::cmd::DynamicQuery<moss::With<Position>>::init(key).pool(entities);
    pos.x = 10; pos.y = 30;
}

void PlayerMovement::tick(const moss::Key<moss::key::READ>& key, const moss::DynamicView& entities) {
    /* Usage can either be DynamicQuery, like in build, or now a usage of Query */
    auto [pos] = moss::cmd::Query<
        moss::With<Position>,
        moss::cmd::View<moss::Include<PlayerTag>>
    >::init(key).pool();

    pos.x++; pos.y -= 2;
    CHECK(pos.x == 11);
    CHECK(pos.y == 28);

    moss::cmd::Quit::init(key).quit();
}

void Player::init(moss::Mirror& mirror) {
    mirror.create()
        .attach<Position, PlayerTag>()
        .connect<PlayerMovement>();
}
