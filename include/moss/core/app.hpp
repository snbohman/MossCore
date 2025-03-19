/*
core/app.hpp - parentless

Core application. An functional
driven approach.

- set values
- systems
- packages

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

    template<typename T>
    App& attachComponent(std::initializer_list<entt::entity> entities = { });
    App& attachSystem(const System& system, std::initializer_list<entt::entity> entities = { });
    template<typename T> App& attachPackage(std::function<const Contex&> package);
    template<typename T> App& attachContainer(std::function<const Contex&> package);

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
