/*
ecs/renderable.cpp - parentless

Impplements renderable.hpp

*/

#include <moss/includes.hpp>
#include <moss/defines.hpp>
#include <moss/ecs/renderable.hpp>
#include <moss/ecs/components.hpp>
#include <moss/utils.hpp>


void moss::RectRenderable::render(moss::SystemTickCrate crate) {
    auto& transform = crate.registry.get<moss::RectTransform>(crate.entity);
    auto& material = crate.registry.get<moss::Material>(crate.entity);
    auto color = raylib::ColorFromNormalized(utils::glmRaylib::glmToRaylib<glm::vec4, raylib::Vector4>(material.color));

    raylib::DrawRectanglePro(
        utils::glmRaylib::glmToRect(transform.position, transform.size),
        utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(transform.position),
        transform.rotation,
        color
    );
}

void moss::CircleRenderable::render(moss::SystemTickCrate crate) {
    auto& transform = crate.registry.get<moss::CircleTransform>(crate.entity);
    auto& material = crate.registry.get<moss::Material>(crate.entity);
    auto color = raylib::ColorFromNormalized(utils::glmRaylib::glmToRaylib<glm::vec4, raylib::Vector4>(material.color));

    raylib::DrawCircleV(
        utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(transform.position),
        transform.radius,
        color
    );
}

void moss::PolyRenderable::render(moss::SystemTickCrate crate) {
    auto& transform = crate.registry.get<moss::PolyTransform>(crate.entity);
    auto& material = crate.registry.get<moss::Material>(crate.entity);
    auto color = raylib::ColorFromNormalized(utils::glmRaylib::glmToRaylib<glm::vec4, raylib::Vector4>(material.color));

    raylib::DrawTriangleFan(
        moss::utils::glmRaylib::glmsToRaylib<glm::vec2, raylib::Vector2>(transform.points).data(),
        transform.points.size(),
        color
    );
}


void moss::RegPolyRenderable::render(moss::SystemTickCrate crate) {
    auto& transform = crate.registry.get<moss::RegPolyTransform>(crate.entity);
    auto& material = crate.registry.get<moss::Material>(crate.entity);
    auto color = raylib::ColorFromNormalized(utils::glmRaylib::glmToRaylib<glm::vec4, raylib::Vector4>(material.color));


    if (material.fill) {
        raylib::DrawPoly(
            utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(transform.position),
            transform.numPoints,
            transform.radius,
            transform.rotation,
            color
        );
    } else {
        raylib::DrawPolyLinesEx(
            utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(transform.position),
            transform.numPoints,
            transform.radius,
            transform.rotation,
            material.thickness,
            color
        );
    }
}
