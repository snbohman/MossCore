/*
core/render.cpp - parentless

Implements default Render Component and
its children.

*/

#pragma once

#include <moss/ecs/componentDefinitions.hpp>
#include <moss/ecs/systems.hpp>


namespace moss {

template<typename S> // S for moss::Sprite
class Renderable : System {
public:
    void tick(moss::SystemTickCrate crate) override;
    virtual void render(S sprite, moss::Material material) { };
};

class RectRenderable : public Renderable<moss::Sprite2D> {
public:
    void render(moss::RectSprite sprite, moss::Material material) override;
};

class CircleRenderable : public Renderable {
public:
    void render(moss::RenderableRenderCrate crate) override;
};

class PolyRenderable : public Renderable {
public:
    void render(moss::RenderableRenderCrate crate) override;
};

class RegPolyRenderable : public Renderable {
public:
    void render(moss::RenderableRenderCrate crate) override;
};

} // moss
