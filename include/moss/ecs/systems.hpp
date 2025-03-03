/*
ecs/systems.hpp - parentless

Defines the standard system base class.

*/

#pragma once

#include <moss/includes.hpp>


namespace moss {

class System {
public:
    System() = default;
    inline virtual ~System() { }

    inline virtual void init() { }
    inline virtual void init(entt::registry& registry) { }
    inline virtual void tick() { }
    inline virtual void tick(entt::registry& registry) { }
    inline virtual void tick(entt::registry& registry, const float& deltaTime) { }
};

} // moss
