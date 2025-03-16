#pragma once

#include <moss/ecs/ecs.hpp>


namespace moss {

class Renderer : public System {
public:
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual bool shouldClose() = 0;
};

} // moss
