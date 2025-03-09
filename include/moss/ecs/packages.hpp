/*
ecs/packages.hpp - parentless

Packages is a easy to use/include struct
that includes all the default components/systems
that a *sprite* may have, as an example.

*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/ecs/components.hpp>
#include <moss/ecs/systems.hpp>


namespace moss {

struct Package {
    ~Package() = default;
};

namespace packages {

struct Physics : Package {
    components::Physics component;
    systems::Physics system;
};

struct Sprite : Package {
    components::Transform transform;
    components::Material material;
    components::Renderable renderable;
    packages::Physics physicsPackage;
};

} // packages
} // moss
