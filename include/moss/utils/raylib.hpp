/*
utils/raylib.hpp

Mostly popyulates the utils::raylib
namespace with glm->raylib functions.

*/

#pragma once

#include <moss/meta/defines.hpp>
#include <moss/meta/libs.hpp>
#include <moss/meta/raylibLibs.hpp>
#include <boost/type_index.hpp>


namespace moss::utils::raylib {

template<typename T, typename U>
inline U glmToRaylib(const T& vec) {
    WARN("Unsupported vector type \"{}\"", boost::typeindex::type_id<T>().pretty_name());

    return U(); // for warning purposes, it quits anyway
}

// Specialization for glm::vec2 -> Vector2
template<>
inline Vector2 glmToRaylib<glm::vec2, Vector2>(const glm::vec2& vec) {
    return Vector2 { vec.x, vec.y };
}

// Specialization for glm::vec3 -> Vector3
template<>
inline Vector3 glmToRaylib<glm::vec3, Vector3>(const glm::vec3& vec) {
    return Vector3 { vec.x, vec.y, vec.z };
}

// Specialization for glm::vec4 -> Vector4
template<>
inline Vector4 glmToRaylib<glm::vec4, Vector4>(const glm::vec4& vec) {
    return Vector4 { vec.x, vec.y, vec.z, vec.w };
}

inline Rectangle glmToRect(const glm::vec2& pos, const glm::vec2& size) {
    return { pos.x, pos.y, size.x, size.y };
}

// glm::vecX, VectorX
template<typename T, typename U>
inline std::vector<U> glmsToRaylib(const std::vector<T>& vec) {
    const glm::u8 size = vec.size();

    std::vector<U> rVec; rVec.reserve(size);
    for (glm::u8 i; i < size; i++) {
        rVec[i] = glmToRaylib<T, U>(vec[i]);
    }

    return rVec;
}

} // utils::raylib
