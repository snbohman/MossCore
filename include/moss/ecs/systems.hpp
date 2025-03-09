/*
ecs/systems.hpp - parentless

Defines the standard system base class.

*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/ecs/components.hpp>


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
    ~System() = default;

    inline virtual void init() { }
    inline virtual void tick() { }
    inline virtual void exit() { }
    inline virtual void init(SystemInitCrate crate) { }
    inline virtual void tick(SystemTickCrate crate) { }
    inline virtual void exit(SystemExitCrate crate) { }
};

///////////////////////////////
//// -- Default systems -- ////
///////////////////////////////
namespace systems {

class Physics : public System { };

} // systems
} // moss
