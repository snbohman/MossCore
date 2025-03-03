/*
core/render.cpp - App

Implements render.hpp

TODO: implement a mesh/renderComponent-something
to easier render sprites. Maybe an attatched
raylib::draw function, a lambda?
*/


#include <moss/includes.hpp>
#include <moss/core/render.hpp>
#include <moss/ecs/components.hpp>
#include <moss/utils.hpp>


//////////////////////
//// -- Public -- ////
//////////////////////
void moss::Render::tick(entt::registry& registry) {
    raylib::ClearBackground(raylib::BLACK);
    raylib::BeginDrawing();
    
    for (auto entity : registry.view<moss::RectTransform>()) {
        moss::RectTransform& transform = registry.get<RectTransform>(entity);
        moss::Material& material = registry.get<moss::Material>(entity);
        raylib::DrawRectangleV(
            moss::utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(transform.position),
            moss::utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(transform.size),
            raylib::ColorFromNormalized(utils::glmRaylib::glmToRaylib<glm::vec4, raylib::Vector4>(material.color))
        );
    }

    for (auto entity : registry.view<moss::CircleTransform>()) {
        moss::CircleTransform& transform = registry.get<moss::CircleTransform>(entity);
        moss::Material& material = registry.get<moss::Material>(entity);
        raylib::DrawCircleV(
            moss::utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(transform.position),
            transform.radius,
            raylib::ColorFromNormalized(utils::glmRaylib::glmToRaylib<glm::vec4, raylib::Vector4>(material.color))
        );
    }

    for (auto entity : registry.view<moss::PolyTransform>()) {
        moss::PolyTransform& transform = registry.get<moss::PolyTransform>(entity);
        moss::Material& material = registry.get<moss::Material>(entity);
        raylib::DrawTriangleFan(
            moss::utils::glmRaylib::glmsToRaylib<glm::vec2, raylib::Vector2>(transform.points).data(),
            transform.points.size(),
            raylib::ColorFromNormalized(utils::glmRaylib::glmToRaylib<glm::vec4, raylib::Vector4>(material.color))
        );
    }

    for (auto entity : registry.view<RegPolyTransform>()) {
        moss::RegPolyTransform& transform = registry.get<moss::RegPolyTransform>(entity);
        moss::Material& material = registry.get<moss::Material>(entity);
        if (material.fill) {
            raylib::DrawPoly(
                moss::utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(transform.position),
                transform.numPoints, transform.radius, transform.rotation,
                raylib::ColorFromNormalized( { material.color.r, material.color.g, material.color.b, material.color.a } )
            );
        } else {
            raylib::DrawPolyLinesEx(
                moss::utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(transform.position),
                transform.numPoints, transform.radius, transform.rotation, material.thickness,
                raylib::ColorFromNormalized( { material.color.r, material.color.g, material.color.b, material.color.a } )
            );
        }
    }

    raylib::EndDrawing();
}
