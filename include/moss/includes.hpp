/*
includes.hpp - parentless

Used for easy includes of used libaries.

*/

#pragma once

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

#include <nlohmann/json.hpp>
using json=nlohmann::json;

namespace rl {
    #include <rlgl.h>
}

namespace raylib {
    #include <raylib.h>
}
