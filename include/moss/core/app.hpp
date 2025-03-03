/*
core/app.hpp - parentless

Core application.

*/

#pragma once

#include <moss/core/scene.hpp>
#include <moss/core/render.hpp>
#include <moss/defines.hpp>
#include <memory>


namespace moss {

class App {
public:
    App();
    ~App();
    void run();

    void addScene(const char* id, const bool& currentScene);
    void buildComponentRegistry(moss::types::ComponentRegistry& componentRegistry);
    void setComponentRegistry(const moss::types::ComponentRegistry& componentRegistry);

private:
    // -- These are only used by app (eventhough they seem public) -- //
    // -- The components should have no access to these -- //
    std::unique_ptr<moss::Scene>& getScene(const char* id);
    std::unique_ptr<moss::Scene>& getCurrentScene();
    std::unique_ptr<moss::Render>& getRender();

    entt::registry m_registry;
    moss::types::ComponentRegistry m_componentRegistry;
};

} // moss
