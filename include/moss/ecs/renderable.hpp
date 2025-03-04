/*
core/render.cpp - parentless

Implements default Render Component and
its children.

*/

#pragma once

#include <moss/ecs/systems.hpp>


namespace moss {

class Renderable {
public:
    Renderable() = default;
    inline virtual ~Renderable() { }

    virtual void render(moss::SystemTickCrate crate) { }
};

class RectRenderable : public Renderable {
public:
    void render(moss::SystemTickCrate crate) override;
};

class CircleRenderable : public Renderable {
public:
    void render(moss::SystemTickCrate crate) override;
};

class PolyRenderable : public Renderable {
public:
    void render(moss::SystemTickCrate crate) override;
};

class RegPolyRenderable : public Renderable {
public:
    void render(moss::SystemTickCrate crate) override;
};

} // moss
