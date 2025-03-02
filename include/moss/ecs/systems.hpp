#pragma once

#include <moss/includes.hpp>


namespace moss {

class System {
public:
    virtual ~System() = default;

    virtual void init() { };
    virtual void init(entt::registry& registry) { };
    virtual void tick() { };
    virtual void tick(entt::registry& registry) { };
    virtual void tick(entt::registry& registry, const float& deltaTime) { };
};

} // moss
