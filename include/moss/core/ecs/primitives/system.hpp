#pragma once

#include <moss/core/contex.hpp>


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
};
