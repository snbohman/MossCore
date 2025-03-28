/*
core/ecs/contex.hpp

auto components = contex.read<Query<
    With<...>,
    View<
        Inlcude<...>,
        Exclude<...>
    >
>();

auto entities = contex.read<View<
    Include<...>,
    Exclude<...>
>();

                -- Or --
auto components = contex
    .read()
        .view<Include<...>, Exclude<...>>()
        .query<With<...>>()
        .pool()
    .save();


                -- Or --
View<...> v;
auto enttView = contex.apply(v);

Query<...> q;
auto [...] = contex.pool(q);

The last one is best because of compile time and
resuable query objects, only requiring read permissions
when applying a view or getting atlas or pool from query.

Should components also be like this? So every component
is compile-time defined. So like

Component<vec3, vec2, float>
instead of
Component {
    vec3 bla;
    vec2 blo;
    float ble;
}

Advantages:
    - Compile time
    - Fits with read standard

Disadvantages:
    - No names (or harder at least)

Conclusion:
    NO. https://chatgpt.com/share/67e6d9ab-0f28-8000-8295-8bfb7263c812

*/

#pragma once

#include <moss/core/app.hpp>
#include <moss/core/ecs/read.hpp>


namespace moss{

namespace contex {

/*
WRITE: The lowest of permissions. You can only write to your
own entity, and not read from others.


READ: Persmissions granted to tick system funcs. All functions 
accessable. You can *read* other entities with views and queries,
and you can write to your own entity or to others.

*/
enum Permissions {
    READ,
    WRITE
};

} // contex

template<contex::Permissions P>
class Contex {
public:
    explicit Contex(App* app) : m_app(app) {}

    Contex(const Contex&) = delete;
    Contex& operator=(const Contex&) = delete;
    Contex(Contex&&) = delete;
    Contex& operator=(Contex&&) = delete;

    Contex& create(glm::u32 count = 1) { m_app->create(count); return *this; }
    template<typename... T> Contex& attachComponent() { m_app->attachComponent<T...>(); return *this; }
    template<typename... T> Contex& attachSystem() { m_app->attachSystem<T...>(); return *this; };
    template<typename... T> Contex& attachPackage() { /* attache package */ return *this; };

private:
    App* m_app;
};

} // namsespace moss
