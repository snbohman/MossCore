/*
packages/primitives.hpp

A package implements entities, attatches systems
to entities, attatches components to entities
and initializes components to entities.

It really is a system, but with a specific purpose.

*/

#include <moss/core/ecs.hpp>


namespace moss {

class Package : public System {
    virtual ~Package() = default;
};

} // moss
