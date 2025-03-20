/*
core/app.hpp - parentless

Core application.

*/

#pragma once


#include <moss/meta/libs.hpp>
#include <moss/core/ecs.hpp>

namespace moss {

class App {
public:
    inline App() { };
    inline ~App() { };

    App& create(glm::u32 count = 1);
    template<typename... T> App& attachComponent();
    template<typename... T> App& attachSystem();
    template<typename... T> App& attachPackage();
    App& query();
    App& view();

    static App& init();
    App& build();
    App& run();
    void exit();

private:
    entt::registry m_registry;
    std::vector<entt::entity> m_latest;

    bool m_quit;
};

} // moss
