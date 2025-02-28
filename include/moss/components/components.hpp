#pragma once

#include <moss/includes.hpp>

namespace moss {

struct Component {
    virtual ~Component() = default;
};

struct RectTransform : Component {
    glm::f32vec2 position;
    glm::f32vec2 size;
    glm::f32 rotation;
};

struct CircleTransform : Component {
    glm::f32vec2 position;
    glm::f32 radius;
};

struct PolyTransform : Component {
    std::vector<glm::f32vec2> points;
};

struct RegPolyTransform : Component {
    glm::f32vec2 position;
    glm::u16 numPoints;
    glm::f32 radius;
    glm::f32 rotation;
};

struct Physics : Component {
    glm::f32vec2 velocity;
    glm::f32vec2 acceleration;
    glm::f32 elasticity;
    glm::f32 mass;
};

struct Material : Component {
    glm::f32vec4 color;
    glm::f32 thickness;
    bool fill;
};

} // moss
