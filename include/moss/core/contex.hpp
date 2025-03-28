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
