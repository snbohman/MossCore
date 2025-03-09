/*
ecs/components.hpp - parentless

Defines all of the standard components and the
standard component base class.

*/

#pragma once

#include <moss/meta/libs.hpp>


namespace moss {

/////////////////////////////
//// -- Base Classses -- ////
/////////////////////////////
struct Component {
    virtual ~Component() = default;
};

struct Renderable : Component { };

struct SphereRenderable : Renderable {
};

////////////////////
//// -- User -- ////
////////////////////
namespace components {

struct Transform : Component {
    glm::f32vec2 position;
    glm::f32vec2 scale;
    glm::f32 rotation;
};

struct Physics : Component {
    glm::f32vec2 velocity;
    glm::f32vec2 acceleration;
    glm::f32 elasticity;
    glm::f32 mass;
};

struct Material : Component {
    glm::f32vec4 albedo;
    glm::f32 thickness;
    bool fill;
};

//////////////////////
//// -- Engine -- ////
//////////////////////
namespace engine {

struct CurrentSceneTag : Component { };

} // engine 

} // components 

} // moss
