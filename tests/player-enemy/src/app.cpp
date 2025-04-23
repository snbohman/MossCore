#include <doctest.h>
#include <moss/moss.hpp>
#include <player.hpp>


TEST_CASE("Basic App") {
    moss::App::instance()
        .init()
        .mount<Player>()
        .build()
        .run()
        .exit();
}
