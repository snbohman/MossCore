/*
ecs/components.hpp

Defines all of the standard components.

A component is simply an ecs/entt component
attatched to an entity. Could technically
be tags, systems, packages or renderables
as they are handled the same in moss,
but for the user they are vastly different.

Components that use a transform is always
relative to the global transform component
of a entity. If a the higher up transform
component doesnt exist, a error is called.
These components *need* a higher up transform.

*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/ecs/primitives.hpp>


namespace moss::components {

////////////////////
//// -- Base -- ////
////////////////////
struct Transform : Component {
    glm::f32vec2 position;
    glm::f32vec2 scale;
    glm::f32 rotation;
};

struct RigidBody : Component {
    glm::f32vec2 velocity;
    glm::f32vec2 acceleration;
    glm::f32 mass;
    glm::f32 elasticity;
};

struct RectCollider : Component {
    Transform transform;
};

struct Material : Component {
    glm::u32vec4 albedo;
    glm::f32 thickness;
    bool fill;
};


//////////////////////
//// -- Engine -- ////
//////////////////////
namespace engine {

struct CurrentSceneTag : Component { };

} // engine 

} // moss::components 
