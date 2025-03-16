/*
render/mrls.cpp

Main hub for implementing mrls.

*/


#include <moss/render/mrls/mrls.hpp>
#include <moss/meta/defines.hpp>
#include <moss/utils/utils.hpp>


using namespace moss::render;

void MRLS::init() {
    json windowConfig; utils::config::readConfig(windowConfig, "windowConfig.json");
    SetTraceLogLevel(LOG_WARNING);

    InitWindow(
        windowConfig["dimensions"][0].get<glm::u32>(),
        windowConfig["dimensions"][1].get<glm::u32>(),
        windowConfig["title"].get<std::string>().c_str()
    );
    SetTargetFPS(windowConfig["targetFPS"].get<glm::f32>());
}

void MRLS::tick(sys::TickCrate crate) {
    BeginDrawing();
    ClearBackground(BLACK);

    /*for (const auto& e : crate.registry.view<std::unique_ptr<Renderable>>()) {*/
    /*    auto& renderable = crate.registry.get<std::unique_ptr<Renderable>>(e);*/
    /**/
    /*    if (auto* circle = dynamic_cast<rcmp::Circle*>(renderable.get())) {*/
    /*        drawRenderable(circle);*/
    /*    } else {*/
    /*        drawRenderable(renderable);*/
    /*    }*/
    /*}*/

    DrawCircleV(
        {0, 0},
        10,
        {1, 0, 0, 1}
    );

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

void MRLS::drawRenderable(const rcmp::Circle* renderable) {
    DrawCircleV(
        utils::raylib::glmToRaylib<glm::f32vec2, Vector2>(renderable->transform.position),
        renderable->shape.radius,
        ColorFromNormalized(utils::raylib::glmToRaylib<glm::u32vec4, Vector4>(renderable->material.albedo))
    );
}
