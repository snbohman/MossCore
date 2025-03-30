#pragma once

#include <moss/meta/libs.hpp>
#include <moss/commands/primitives.hpp>

namespace moss {

namespace fluent {

enum Permissions {
    READ = 1 << 0,
    WRITE = 1 << 1,
};

}

template<fluent::Permissions P>
class Fluent {
public:
    Fluent(const Fluent&) = delete;
    Fluent& operator=(const Fluent&) = delete;
    Fluent(Fluent&&) = delete;
    Fluent& operator=(Fluent&&) = delete;

    static Fluent& init();
    static Fluent& get();

    ///////////////////
    //// -- ECS -- ////
    ///////////////////
    void create(glm::u32 count = 1);
    void view();
    void query();

    ///////////////////////////////
    //// -- ECS (Templated) -- ////
    ///////////////////////////////
    template<typename Components, typename View> Fluent& attach();

    template<typename C..., typename Vw...> Fluent<View<Vw...>> & attach() {
        for (entt::entity entity : m_view) m_registry->emplace<T...>(entity);
        return *this;
    }

    template<typename... T> Fluent& attach() {
        for (entt::entity entity : m_view) m_registry->emplace<T...>(entity);
        return *this;
    }
 
private:
    entt::registry* m_registry;
    commands::write::DynamicView m_view;
};

}
