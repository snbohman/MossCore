/*
ecs/systems.hpp - parentless

Defines the standard system base class.

*/

#pragma once

#include <moss/meta/libs.hpp>


namespace moss {

struct SystemInitCrate {
    const entt::entity& entity = entt::null;
};

struct SystemTickCrate {
    const entt::entity& entity = entt::null;
    entt::registry& registry;
    const glm::f32& deltaTime = -1; // undefined when not defined otherwise
};

struct SystemExitCrate {
    const entt::entity& entity = entt::null;
    entt::registry& registry;
};


class System {
public:
    System() = default;
    inline virtual ~System() { }

    inline virtual void init(moss::SystemInitCrate crate) { }
    inline virtual void tick(moss::SystemTickCrate crate) { }
    inline virtual void exit(moss::SystemExitCrate crate) { }
};

} // moss
