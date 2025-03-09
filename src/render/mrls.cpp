/*
render/mrls.cpp

Implements render/mrls.hpp

*/

#include <moss/render/mrls.hpp>
#include <fstream>

using namespace moss::render;

void MRLS::init() {
    windowConfig = json::parse(std::ifstream("data/windowConfig.json"));

    InitWindow(
        windowConfig["width"].get<glm::f32>(),
        windowConfig["height"].get<glm::f32>(),
        windowConfig["title"].get<std::string>().c_str()
    );
    SetTargetFPS(windowConfig["targetFPS"].get<glm::f32>());
}

bool MRLS::shouldClose() {
    return WindowShouldClose();
}

/* -- Draw -- */
void MRLS::beginDrawing() { BeginDrawing(); }
void MRLS::endDrawing()   { EndDrawing(); }
