/*
utils/math.hpp - parentless

Implements utils::math with relevant helper functions.

*/

#pragma once

#include <moss/meta/libs.hpp>


namespace moss::utils::math {

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

}
