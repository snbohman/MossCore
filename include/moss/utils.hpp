/*

Core utils namespace(s). Includes the essentials.
The utils namespace might be added to in proj.

*/

#pragma once

#include <moss/includes.hpp>


namespace moss {

namespace utils {

namespace math {
    inline glm::f32 length(const glm::vec2& v) {
        return distance(v, glm::f32vec2(0, 0));
    }

    inline glm::f32 distance(const glm::vec2& p1, const glm::vec2& p2) {
        return glm::sqrt(glm::pow((p1.x - p2.x), 2) + glm::pow((p1.y - p2.y), 2));
    }

    inline glm::f32 distancePointToLine(const glm::f32vec2& point, const glm::f32vec2& lineStart, const glm::f32vec2& lineEnd) {
        glm::f32vec2 lineDir = {lineEnd.x - lineStart.x, lineEnd.y - lineStart.y};
        glm::f32vec2 pointDir = {point.x - lineStart.x, point.y - lineStart.y};

        float t = (pointDir.x * lineDir.x + pointDir.y * lineDir.y) / (lineDir.x * lineDir.x + lineDir.y * lineDir.y);
        t = std::max(0.0f, std::min(1.0f, t));

        glm::f32vec2 closestPoint = {lineStart.x + t * lineDir.x, lineStart.y + t * lineDir.y};
        float dx = point.x - closestPoint.x;
        float dy = point.y - closestPoint.y;
        return glm::f32(glm::sqrt(dx * dx + dy * dy));
    }
} // Math

namespace glmRaylib {
    template<typename T, typename U>
    inline U glmToRaylib(const T& vec) {
        static_assert(sizeof(T) == 0, "Unsupported vector type for glmToRaylib");
    }

    // Specialization for glm::vec2 -> Vector2
    template<>
    inline raylib::Vector2 glmToRaylib<glm::vec2, raylib::Vector2>(const glm::vec2& vec) {
        return raylib::Vector2{vec.x, vec.y};
    }

    // Specialization for glm::vec3 -> raylib::Vector3
    template<>
    inline raylib::Vector3 glmToRaylib<glm::vec3, raylib::Vector3>(const glm::vec3& vec) {
        return raylib::Vector3{vec.x, vec.y, vec.z};
    }

    // Specialization for glm::vec4 -> raylib::Vector4
    template<>
    inline raylib::Vector4 glmToRaylib<glm::vec4, raylib::Vector4>(const glm::vec4& vec) {
        return raylib::Vector4{vec.x, vec.y, vec.z, vec.w};
    }

    // glm::vecX, raylib::VectorX
    template<typename T, typename U>
    inline std::vector<U> glmsToRaylib(const std::vector<T>& vec) {
        const glm::u8 size = vec.size();

        std::vector<U> rVec; rVec.reserve(size);
        for (glm::u8 i; i < size; i++) {
            rVec[i] = glmToRaylib<T, U>(vec[i]);
        }

        return rVec;
    }
} // glmRaylib
} // utils
} // mossk
