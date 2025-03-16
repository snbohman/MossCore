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

struct InitCrate {
    const entt::entity& entity = entt::null;
};

struct TickCrate {
    const entt::entity& entity = entt::null;
    entt::registry& registry;
};

struct ExitCrate {
    const entt::entity& entity = entt::null;
    entt::registry& registry;
};

} // systems

class System {
public:
    System() = default;
    virtual ~System() = default;

    inline virtual void init() { }
    inline virtual void tick() { }
    inline virtual void exit() { }
    inline virtual void init(systems::InitCrate crate) { }
    inline virtual void tick(systems::TickCrate crate) { }
    inline virtual void exit(systems::ExitCrate crate) { }
};

} // moss
