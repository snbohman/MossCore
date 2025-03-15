#pragma once

#include <moss/ecs/ecs.hpp>


namespace moss {

class Renderer : public System {
public:
    virtual bool shouldClose();
};

} // moss
