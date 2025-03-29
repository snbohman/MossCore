/*
core/app.hpp - parentless

Core application.

Maybe move the attach stuff too
it's own logic, like contex plans.

*/

#pragma once


#include <moss/meta/libs.hpp>
#include <moss/core/ecs/component.hpp>
#include <moss/core/commands/primitives.hpp>

namespace moss {

namespace contex {

enum Permissions {
    READ = 1 << 0,
    WRITE = 1 << 1,
};

}

template<contex::Permissions P>
class Contex {
public:
    explicit Contex() { }

    Contex& create(glm::u32 count = 1);

    Contex(const Contex&) = delete;
    Contex& operator=(const Contex&) = delete;
    Contex(Contex&&) = delete;
    Contex& operator=(Contex&&) = delete;

    Contex& quit();

    static Contex& init();
    Contex& build();
    Contex& run();
    void exit();

    ///////////////////////////////
    //// -- ECS (Templated) -- ////
    ///////////////////////////////
    template<typename... T>
    Contex& attachComponent() {
        for (entt::entity entity : m_latest) {
            m_registry.emplace<T...>(entity);
        }

        return *this;
    }

    template<typename... T>
    Contex& attachSystem() {
        for (entt::entity entity : m_latest) {
            m_registry.emplace<T...>(entity).init();
        }

        return *this;
    }

private:
    entt::registry m_registry;
    DynamicView m_latest;

    bool m_quit = false;
};

} // moss
