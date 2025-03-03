/*
includes.hpp - parentless

Used for easy includes of used libaries.

*/

#pragma once

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

namespace raylib {
    #include <raylib.h>
}


#include <nlohmann/json.hpp>
using json=nlohmann::json;
