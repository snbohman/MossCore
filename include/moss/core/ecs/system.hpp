#pragma once

#include <moss/core/contex.hpp>
#include <moss/core/commands/primitives.hpp>

namespace moss {

class System {
public:
    virtual ~System() = default; 

    virtual void init() { }
    virtual void build() { }
    virtual void tick() { }
    virtual void exit() { }

    virtual void build(Contex<contex::WRITE>& contex) { }
    virtual void tick(Contex<contex::READ>& contex) { }
    virtual void exit(Contex<contex::WRITE>& contex) { }

    // Entity View is always of size one, but provided in vector format 
    // for consintensies sake with querying stuff.
    virtual void build(Contex<contex::WRITE>& contex, const DynamicView& entityView) { }
    virtual void tick(Contex<contex::READ>& contex, const DynamicView& entityView) { }
    virtual void exit(Contex<contex::WRITE>& contex, const DynamicView& entityView) { }
};

}
