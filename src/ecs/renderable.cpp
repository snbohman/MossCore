/*
ecs/renderable.cpp - parentless

Impplements renderable.hpp

*/

#include <moss/includes.hpp>
#include <moss/defines.hpp>
#include <moss/ecs/renderable.hpp>
#include <moss/ecs/components.hpp>
#include <moss/ecs/systems.hpp>
#include <moss/utils.hpp>
#include <raylib.h>


void moss::Renderable::tick(moss::SystemTickCrate crate) {
    raylib::Camera2D& camera = crate.registry.get<raylib::Camera2D>(*crate.registry.view<raylib::Camera2D>().begin());
    moss::Sprite2D& sprite = crate.registry.get<moss::Sprite2D>(crate.entity);
    moss::Material& material = crate.registry.get<moss::Material>(crate.entity);

    raylib::BeginMode2D(camera);

    rl::rlPushMatrix();
    rl::rlTranslatef(sprite->transform.position.x, sprite.transform.position.y);

    render({ sprite, material });
    raylib::EndMode2D();
}

void moss::RectRenderable::render(moss::RenderableRenderCrate crate) {
    raylib::DrawRectanglePro(
        utils::glmRaylib::glmToRect(crate.transform.position, crate.transform.),
        utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(crate.transform.position),
        crate.transform.rotation,
        crate.material.albedo
    );
}

void moss::CircleRenderable::render(moss::RenderableRenderCrate crate) {
    raylib::DrawCircleV(
        utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(crate.transform.position),
        crate.transform.radius,
        crate.color
    );
}

void moss::PolyRenderable::render(moss::RenderableRenderCrate crate) {
    raylib::DrawTriangleFan(
        moss::utils::glmRaylib::glmsToRaylib<glm::vec2, raylib::Vector2>(crate.transform.points).data(),
        crate.transform.points.size(),
        crate.color
    );
}


void moss::RegPolyRenderable::render(moss::RenderableRenderCrate crate) {
    if (material.fill) {
        raylib::DrawPoly(
            utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(crate.transform.position),
            crate.transform.numPoints,
            crate.transform.radius,
            crate.transform.rotation,
            crate.color
        );
    } else {
        raylib::DrawPolyLinesEx(
            utils::glmRaylib::glmToRaylib<glm::vec2, raylib::Vector2>(crate.transform.position),
            crate.transform.numPoints,
            crate.transform.radius,
            crate.transform.rotation,
            material.thickness,
            crate.color
        );
    }
}
