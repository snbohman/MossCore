/*
render/mlrs.hpp

Thin class/renderSystem wrapper around raylib,
simplified and specialized for moss.

Use this if drawing to screen without objects
is of need, and if performance is neglected/
discarded in place of efficiency and ease
of use.

*/

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/raylibLibs.hpp>
#include <moss/render/render.hpp>


namespace moss::render {

class MRLS : Renderer {
public:
    void init() override;
    void tick() override;
    void exit() override;
    bool shouldClose() override;

    // - Handy API - //
    void beginDrawing();
    void endDrawing();

private:
    json windowConfig;
};

} // moss::render
