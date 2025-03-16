/*
render/mrls.cpp

Main hub for implementing mrls.

*/


#include <moss/render/mrls/mrls.hpp>
#include <moss/meta/defines.hpp>
#include <moss/utils/utils.hpp>


using namespace moss::render;

void MRLS::init() {
    INFO_INITF("Initializing MRLS renderer", 1);

    INFO_INITF("Reading mrlsConfig.json", 2);
    json mrlsConfig; utils::config::readConfig(mrlsConfig, "mrlsConfig.json");

    INFO_INITF("Reading windowConfig.json", 2);
    json windowConfig; utils::config::readConfig(windowConfig, "windowConfig.json");
    const auto width = windowConfig["dimensions"][0].get<glm::u32>();
    const auto height = windowConfig["dimensions"][1].get<glm::u32>();
    const auto title = windowConfig["title"].get<std::string>();
    const auto targetFPS = windowConfig["targetFPS"].get<glm::f32>();

    INFO_INITF("Setting raylib trace level to WARN", 2);
    SetTraceLogLevel(LOG_WARNING);

    INFO_INIT("Initializing Raylib Window with dimensions [{}-{}] and title \"{}\"", 2, width, height, title)
    InitWindow(width, height, title.c_str());

    INFO_INIT("Setting target-FPS to {}", 2, targetFPS);
    SetTargetFPS(targetFPS);
}

void MRLS::tick(sys::TickCrate crate) {
    BeginDrawing();
    ClearBackground(BLACK);

    for (const auto& e : crate.registry.view<std::unique_ptr<Renderable>>()) {
        auto& renderable = crate.registry.get<std::unique_ptr<Renderable>>(e);

        if (auto* circle = dynamic_cast<rcmp::Circle*>(renderable.get())) {
            drawRenderable(circle);
        } else {
            drawRenderable(renderable);
        }
    }

    EndDrawing();
}

void MRLS::exit() { CloseWindow(); }
bool MRLS::shouldClose() { return WindowShouldClose(); }

////////////////////
//// -- DRAW -- ////
////////////////////
void MRLS::drawRenderable(const std::unique_ptr<Renderable>& renderable) {
    WARN("Renderable render call with input \"{}-renderable\" not defined", boost::typeindex::type_id<decltype(*renderable)>().pretty_name());
}

void MRLS::drawRenderable(const rcmp::Circle* renderable) {
    DrawCircleV(
        utils::raylib::glmToRaylib<glm::f32vec2, Vector2>(renderable->transform.position),
        renderable->shape.radius,
        ColorFromNormalized(utils::raylib::glmToRaylib<glm::f32vec4, Vector4>(renderable->material.albedo))
    );
}
