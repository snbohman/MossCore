/*ecs
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

    virtual void init(Contex<contex::ATTACH>& contex) { }
    virtual void build(Contex<contex::CREATE>& contex) { }
    virtual void tick(Contex<contex::CREATE>& contex) { }
    virtual void exit(Contex<contex::READ>& contex) { }
};

class Package {
public:
    virtual ~Package() = default;

    virtual void init(Contex<contex::ATTACH>& contex) { }
    void build(Contex<contex::CREATE> & contex);
    void tick(Contex<contex::CREATE> & contex);
    void exit(Contex<contex::READ> & contex);
};

} // moss

