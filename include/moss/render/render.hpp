#pragma once

#include <moss/ecs/systems.hpp>


namespace moss {

class Renderer : public moss::System {
public:
    virtual bool shouldClose();
};

} // moss
