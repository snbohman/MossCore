#pragma once

#include <moss/core/contex.hpp>


class Package {
public:
    virtual ~Package() = default;

    virtual void init() { }
    void build(Contex<contex::WRITE>& contex);
    void tick(Contex<contex::READ>& contex);
    void exit(Contex<contex::WRITE>& contex);
};
