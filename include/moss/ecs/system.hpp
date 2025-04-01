#pragma once

#include <moss/core/contex.hpp>
#include <moss/commands/primitives.hpp>

namespace moss {

class System {
public:
    virtual ~System() = default; 

    virtual void init() { }
    virtual void build() { }
    virtual void tick() { }
    virtual void exit() { }
};

}
