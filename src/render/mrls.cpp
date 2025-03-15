/*
render/mrls.cpp

Main hub for implementing mrls.

*/


#include <moss/render/mrls/mrls.hpp>
#include <moss/utils/utils.hpp>


using namespace moss::render;

void MRLS::init() {
    json windowConfig; utils::config::readConfig(windowConfig, "data/windowConfig.json");

    InitWindow(
        windowConfig["dimensions"]["width"].get<glm::u32>(),
        windowConfig["dimensions"]["height"].get<glm::u32>(),
        windowConfig["title"].get<std::string>().c_str()
    );
    SetTargetFPS(windowConfig["targetFPS"].get<glm::f32>());
}

void MRLS::tick(sys::TickCrate crate) {
    BeginDrawing();

    for (const auto& e : crate.registry.view<std::unique_ptr<Renderable>>()) {
        drawRenderable(crate.registry.get<std::unique_ptr<Renderable>>(e));
    }

    EndDrawing();
}

void MRLS::exit() { CloseWindow(); }
bool MRLS::shouldClose() { return WindowShouldClose(); }

////////////////////
//// -- DRAW -- ////
////////////////////
void MRLS::drawRenderable(const std::unique_ptr<Renderable>& renderable) {
    WARN("Renderable draw call with type input \"{}-renderable\" not defined", boost::typeindex::type_id<decltype(*renderable)>().pretty_name());
}

void MRLS::drawRenderable(const std::unique_ptr<rnd::Circle>& renderable) {
    DrawCircleV(
        utils::raylib::glmToRaylib<glm::f32vec2, Vector2>(renderable->transform.position),
        renderable->shape.radius,
        ColorFromNormalized(utils::raylib::glmToRaylib<glm::u32vec4, Vector4>(renderable->material.albedo))
    );
}
