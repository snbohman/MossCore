/*
ecs/componentDefinitions.hpp - parentless

Defines all of the standard components and the
standard component base class.

*/

#pragma once

#include <moss/includes.hpp>

namespace moss {

/* -- Base Classes -- */
struct Component {
    virtual ~Component() = default;
};

struct Sprite2D : Component {
    virtual ~Sprite2D() = default;
};

/* -- 2D -- */
struct Camera2D : Component {
    glm::f32vec2 position;
    glm::f32vec2 rotation;
    glm::f32vec2 zoom;
};

struct Transform2D : Component {
    glm::f32vec2 position;
    glm::f32vec2 scale;
    glm::f32 rotation;
};

struct RectSprite : Sprite2D {
    Transform2D transform;

    glm::f32 thickness;
    bool fill;
};

struct CircleSprite : Sprite2D {
    Transform2D transform;

    glm::f32 radius;
    glm::f32 thickness;
    bool fill;
};

struct PolySprite : Sprite2D {
    Transform2D transform;

    std::vector<glm::f32vec2> points;
    glm::f32 thickness;
    bool fill;
};

struct RegPolySprite : Sprite2D {
    Transform2D transform;

    glm::u16 numPoints;
    glm::f32 radius;
    glm::f32 thickness;
    bool fill;
};

/* -- Other -- */
struct Physics : Component {
    glm::f32vec2 velocity;
    glm::f32vec2 acceleration;
    glm::f32 elasticity;
    glm::f32 mass;
};

struct Material : Component {
    glm::f32vec4 albedo;
};

/* -- Engine Usage -- */
struct CurrentSceneTag : Component { };

} // moss
