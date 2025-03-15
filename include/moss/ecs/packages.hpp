/*
ecs/packages.hpp - parentless

Packages is a easy to use/include struct
that includes all the default components/systems
that a *sprite* may have, as an example.

*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/ecs/primitives.hpp>
#include <moss/ecs/components.hpp>
#include <moss/ecs/systems.hpp>


namespace moss::packages {

struct RigidBody : Package {
    components::RigidBody component;
    systems::RigidBody system;
};

struct Sprite : Package {
    components::Transform transform;
    components::Material material;
    packages::RigidBody physicsPackage;
    Renderable renderable;
};

} // moss::package
