/*
ecs/renderable.hpp

Defines all of the standard renderables.

A renderable is essentially the way to
interact with the renderer. It includes
a transform, material and shape specific
values, such as radius for sphere. The
material *is* attatched to the renderable
component. The renderable component
essentially is just a tag, with relative
transform (more on that below) and a shape
specific struct.

As mentioned earlier, transforms are relative
to the higher up transform. So a [1, 1] position
would grant a render that is

            ent.pos + ent.rend.pos,

so adding them together.

*/

#pragma once

#include <moss/ecs/components.hpp>
#include <moss/ecs/primitives.hpp>


namespace moss::renderables {

struct Circle : Renderable {
    struct Shape {
        float radius;
    };

    components::Transform transform;
    components::Material material;
    Circle::Shape shape;
};

} // moss::renderable
