/*
core/app.cpp - parentless

Implements core/app.hpp.

*/

#include <moss/meta/libs.hpp>
#include <moss/core/app.hpp>
#include <moss/ecs/ecs.hpp>
#include <moss/utils/config.hpp>


using moss::App;

////////////////////
//// -- Init -- ////
////////////////////
App& App::init() { return *this; }

//////////////////////
//// -- Create -- ////
//////////////////////
App& App::create() {
    m_latest = { m_registry.create() };
    return *this;
}

App& App::create(glm::u32 count) {
    for (int i; i < count; i++)
        m_latest.push_back(m_registry.create());
    return *this;
}

/////////////////////////
//// -- Component -- ////
/////////////////////////
template<typename T> App& App::attachComponent() {
    for (const auto& entity : m_latest)
        m_registry.emplace<T>(entity);

    return *this;
}

template<typename T> App& App::attachComponent(entt::entity entity) {
    m_registry.emplace<T>(entity);

    return *this;
}

template<typename T> App& App::attachComponent(std::initializer_list<entt::entity> entities) {
    for (const auto entity : entities)
        m_registry.emplace<T>(entity);

    return *this;
}

//////////////////////
//// -- System -- ////
//////////////////////
App& App::attachSystem(const std::function<const std::unique_ptr<sys::Crate>&>& sys) {
    for (const auto& entity : m_latest) {
        m_registry.emplace<System>(entity, sys);
    }

    return *this;
}

App& App::attachSystem(const std::function<const std::unique_ptr<sys::Crate>&>& sys, entt::entity entity) {
    m_registry.emplace<System>(entity, sys);

    return *this;
}

App& App::attachSystem(const std::function<const std::unique_ptr<sys::Crate>&>& sys, std::initializer_list<entt::entity> entities) {
    for (const auto& entity : entities) {
        m_registry.emplace<System>(entity, sys);
    }

    return *this;
}


App& App::build(app::BuildCrate crate) { return *this; }

App& App::run() {
    // exit on windowclose, etc. should systems be able to call exit?
    while (true) {

    }

    return *this;
}

void App::exit() { }
