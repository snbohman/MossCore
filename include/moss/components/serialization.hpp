#pragma once

#include <moss/includes.hpp>
#include <moss/components/components.hpp>

/// Custom serialization for glm::vec2 ///
namespace glm {
/// - json <-> glm::vec2 - ///
inline void to_json(json& j, const glm::vec2& v) {
    j = json::array({ v.x, v.y });
}

inline void from_json(const json& j, glm::vec2& vec) {
    vec.x = j[0].get<float>();
    vec.y = j[1].get<float>();
}

/// - json <-> glm::vec3 - ///
inline void to_json(json& j, const glm::vec3& v) {
    j = json::array({ v.x, v.y, v.z });
}

inline void from_json(const json& j, glm::vec3& vec) {
    vec.x = j[0].get<float>();
    vec.y = j[1].get<float>();
    vec.z = j[2].get<float>();
}

/// - json <-> glm::vec4 - ///
inline void to_json(json& j, const glm::vec4& v) {
    j = json::array({ v.x, v.y, v.z, v.a });
}

inline void from_json(const json& j, glm::vec4& vec) {
    vec.x = j[0].get<float>();
    vec.y = j[1].get<float>();
    vec.z = j[2].get<float>();
    vec.a = j[3].get<float>();
}

}

namespace moss {

#define SERIALIZE_COMPONENT(component, ...)  NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(component, __VA_ARGS__);

SERIALIZE_COMPONENT(moss::RectTransform, position, size, rotation);
SERIALIZE_COMPONENT(moss::CircleTransform, position, radius);
SERIALIZE_COMPONENT(moss::PolyTransform, points);
SERIALIZE_COMPONENT(moss::RegPolyTransform, position, numPoints, radius, rotation);
SERIALIZE_COMPONENT(moss::Physics, velocity, acceleration, mass, elasticity);
SERIALIZE_COMPONENT(moss::Material, color, thickness, fill);

}
