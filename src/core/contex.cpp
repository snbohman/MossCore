/*
core/app.cpp - parentless

Implements core/app.hpp.

*/

#include <moss/meta/libs.hpp>
#include <moss/core/contex.hpp>


using moss::Contex;


///////////////////
//// -- ECS -- ////
///////////////////
App& App::create(glm::u32 count) {
    m_latest.clear();
    m_latest.reserve(count);

    for (glm::u32 i = 0; i < count; i++) {
        m_latest.push_back(m_registry.create());
    }

    return *this;
}


////////////////////////
//// -- Lifetime -- ////
////////////////////////
App& App::init() {
    static App instance;

    spdlog::set_pattern("[\e[1;32mMOSS\e[0m] [%^%L%$] %v");
    spdlog::set_level(spdlog::level::warn);

    return instance;
}

App& App::build() {
    for (entt::entity system : m_registry.view<System>()) {
        m_registry.get<System>(system).build();
    }

    return *this;
}

App& App::run() {
    while (!m_quit) {
        for (entt::entity system : m_registry.view<System>()) {
            m_registry.get<System>(system).tick();
        }
    }

    return *this;
}

void App::exit() {
    for (entt::entity system : m_registry.view<System>()) {
        m_registry.get<System>(system).exit();
    }
}

App& App::quit() {
    m_quit = true;
    return *this;
}
