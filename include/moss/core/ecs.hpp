/*
ecs/ecs.hpp

Defines the standard/base component,
package and system class.

Primitives are defined in the moss
namespace. If primitives have
dedicated crates, they should be
defined as [...]crate in their
corresponding namespace.

*/

#pragma once


#include <moss/meta/libs.hpp>
#include <moss/core/contex.hpp>


namespace moss {

struct Component {
public:
    virtual ~Component() = default;
};

class System {
public:
    virtual ~System() = default; 

    virtual void init() { }
    virtual void build() { }
    virtual void tick() { }
    virtual void exit() { }

    virtual void build(Contex& contex) { }
    virtual void tick(Contex& contex) { }
    virtual void exit(Contex& contex) { }
};

} // moss

