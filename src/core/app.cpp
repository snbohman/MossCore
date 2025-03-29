/*
core/app.cpp - parentless

Implements core/app.hpp.

*/

#include <moss/meta/libs.hpp>
#include <moss/core/app.hpp>
#include <moss/core/ecs.hpp>
#include <type_traits>


using moss::App;


App& App::quit() {
    m_quit = true;
    return *this;
}

App& App::loadRegistry(entt::registry* registry) {
    registry = &m_registry;
    return *this;
}

///////////////////
//// -- ECS -- ////
///////////////////
template<typename... T>
App& App::attachSystem() {
    static_assert(
        std::is_base_of<T..., System>::value,
        "All type inputs must be a base class of moss::System"
    );

    for (entt::entity entity : m_latest) {
        m_registry.emplace<T...>(entity).init();
    }

    return *this;
}
