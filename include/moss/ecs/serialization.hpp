/*
ecs/serialization.hpp - parentless

Serializes the standard components and 
includes all of the serialization of glm.
Also defines handy macros for user use
when serializing entity attatchements.

*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/ecs/components.hpp>
#include <memory.h>


////////////////////////////////////
//// -- Custom serialization -- ////
////////////////////////////////////
namespace glm {
// -- json <-> glm::vec2 -- //
inline void to_json(json& j, const glm::vec2& v) {
    j = json::array({ v.x, v.y });
}

inline void from_json(const json& j, glm::vec2& vec) {
    vec.x = j[0].get<float>();
    vec.y = j[1].get<float>();
}

// -- json <-> glm::vec3 -- //
inline void to_json(json& j, const glm::vec3& v) {
    j = json::array({ v.x, v.y, v.z });
}

inline void from_json(const json& j, glm::vec3& vec) {
    vec.x = j[0].get<float>();
    vec.y = j[1].get<float>();
    vec.z = j[2].get<float>();
}

// -- json <-> glm::vec4 -- //
inline void to_json(json& j, const glm::vec4& v) {
    j = json::array({ v.x, v.y, v.z, v.a });
}

inline void from_json(const json& j, glm::vec4& vec) {
    vec.x = j[0].get<float>();
    vec.y = j[1].get<float>();
    vec.z = j[2].get<float>();
    vec.a = j[3].get<float>();
}

} // glm


/////////////////////////////////////
//// -- Serialization defines -- ////
/////////////////////////////////////
#define SERIALIZE_COMPONENT(component, ...)  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(component, __VA_ARGS__);

#define FILL_COMPONENT_DATA(component) { \
    #component, \
    [](entt::registry& registry, entt::entity& entity, const json& data) { \
        registry.emplace<component>(entity, data.get<component>()); \
    } \
}

#define FILL_TAG_DATA(tag) { \
    #tag, \
    [](entt::registry& registry, entt::entity& entity, const json& data) { \
        registry.emplace<tag>(entity); \
    } \
}

#define FILL_SYSTEM_DATA(sys) { \
    #sys, \
    [](entt::registry& registry, entt::entity& entity, const json& data) { \
        auto& s = registry.emplace<std::unique_ptr<moss::System>>(entity, std::make_unique<sys>()); \
        s->init(initCrate); \
    } \
}

#define FILL_RENDERABLE_DATA(renderable) { \
    #renderable, \
    [](entt::registry& registry, entt::entity& entity, const json& data) { \
        registry.emplace<std::unique_ptr<moss::Renderable>>(entity, std::make_unique<renderable>()); \
    } \
}

#define REGISTER_COMPONENT(component) componentRegistry[#component] = \
    [](entt::registry& registry, entt::entity& entity, const json& data) { \
        registry.emplace<component>(entity, data.get<component>()); \
    }

#define REGISTER_TAG(tag) componentRegistry[#tag] = \
    [](entt::registry& registry, entt::entity& entity, const json& data) { \
        registry.emplace<tag>(entity); \
    }

#define REGISTER_SYSTEM(sys, initCrate) componentRegistry[#sys] = \
    [](entt::registry& registry, entt::entity& entity, const json& data) { \
        auto& s = registry.emplace<std::unique_ptr<moss::System>>(entity, std::make_unique<sys>()); \
        s->init(initCrate); \
    }

#define REGISTER_RENDERABLE(renderable) componentRegistry[#renderable] = \
    [](entt::registry& registry, entt::entity& entity, const json& data) { \
        registry.emplace<std::unique_ptr<moss::Renderable>>(entity, std::make_unique<renderable>()); \
    }

// define REGISTER_COMPONENT_[TRANSFORM, MATERIAL...]
// ...
// define REGISTER_RENDERER

/////////////////////////////////////////////////
//// -- Standard components serialization -- ////
/////////////////////////////////////////////////
namespace moss {

SERIALIZE_COMPONENT(components::Transform, position, scale, rotation);
SERIALIZE_COMPONENT(components::Physics, velocity, acceleration, mass, elasticity);
SERIALIZE_COMPONENT(components::Material, albedo);

} // moss
