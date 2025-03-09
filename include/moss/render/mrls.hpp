/*
mlrs.hpp - moss

Thin single file wrapper around raylib,
simplified and specialized for moss.

Seperated from moss by include directory, but
still not different projects, as this is only
used very specifically by moss core, as a
single header.

*/

#include <raylib.h>
#include <glm/glm.hpp>

namespace moss::render::mrls {

/* -- Base -- */
inline void init(const glm::f32vec2& dimensions, const char* title, const glm::f32& targetFPS) {
    InitWindow(dimensions.x, dimensions.y, title);
    SetTargetFPS(targetFPS);
}

inline void gameLoop(void *(loop)()) {
    while (!WindowShouldClose()) {
        loop();
    }
}

/* -- Draw -- */
inline void beginDrawing() { BeginDrawing(); }
inline void endDrawing()   { EndDrawing(); }

} // moss::mrls::mrls
