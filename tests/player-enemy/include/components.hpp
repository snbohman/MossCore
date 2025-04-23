#pragma once

#include <moss/ecs/ecs.hpp>


struct Position : moss::Component { float x; float y; };
struct PlayerTag : moss::Component { };
struct EnemyTag : moss::Component { };
