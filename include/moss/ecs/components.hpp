/*
ecs/components.hpp - parentless

Defines all of the standard components and the
standard component base class.

*/

#pragma once

namespace moss {

/* -- Base Classes -- */
struct Component {
    virtual ~Component() = default;
};


/* -- Engine Usage -- */
struct CurrentSceneTag : Component { };

} // moss
