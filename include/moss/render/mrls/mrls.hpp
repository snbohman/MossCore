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
#include <moss/ecs/ecs.hpp>
#include <moss/render/primitives.hpp>

namespace moss::render {

class MRLS : public Renderer {
public:
    void init() override;
    void tick(sys::TickCrate crate) override;
    void exit() override;
    bool shouldClose() override;

private:
    void drawRenderable(const std::unique_ptr<Renderable>& renderable);
    void drawRenderable(const std::unique_ptr<rcmp::Circle>& renderable);

    json windowConfig;
};

} // moss::render
