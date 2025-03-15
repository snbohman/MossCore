/*
ecs/systems.hpp - parentless

Defines the default/included by moss systems,
such as physics.

*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/ecs/components.hpp>


namespace moss::systems {

class RigidBody : public System { };

} // moss::systems
