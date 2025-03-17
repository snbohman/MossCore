/*
ecs/systems.hpp - parentless

Defines the standard/base component,
package and system class.

Primitives are defined in the moss
namespace. If primitives have
dedicated crates, they should be
defined as [...]crate in their
corresponding namespace.

*/

#pragma once


#include <moss/meta/libs.hpp>


namespace moss {

/////////////////////////
//// -- Component -- ////
/////////////////////////
struct Component {
public:
    virtual ~Component() = default;
};


//////////////////////////
//// -- Renderable -- ////
//////////////////////////
struct Renderable : Component {
public:
    virtual ~Renderable() = default;
};


///////////////////////
//// -- Package -- ////
///////////////////////
struct Package {
public:
    virtual ~Package() = default;
};


//////////////////////
//// -- System -- ////
//////////////////////
namespace systems {

struct Crate { };

struct InitCrate : Crate { };

struct BuildCrate : Crate {
    const entt::entity& entity;
    entt::registry& registry;
};

struct TickCrate : Crate {
    const entt::entity& entity;
    entt::registry& registry;
    float deltaTime;
};

struct ExitCrate : Crate {
    const entt::entity& entity;
    entt::registry& registry;
};

} // systems

// Wrapper to make it a component in entt eyes
class System {
    std::function<void()> func;

    void operator () () const {
        func();
    }
};


} // moss
