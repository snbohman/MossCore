#pragma once

#include <moss/core/scene.hpp>
#include <moss/core/render.hpp>
#include <moss/types.hpp>

namespace moss {

class App {
public:
    App();
    ~App();
    void run();

    void addScene(const char* id, const bool& currentScene);
    std::unique_ptr<Scene>& getScene(const char* id);
    std::unique_ptr<Scene>& getCurrentScene();

    void buildComponentRegistry(entt::registry& registry, moss::ComponentRegistry& componentRegistry);
    void setComponentRegistry(moss::ComponentRegistry& componentRegistry);

private:
    std::unique_ptr<Render>& getRender();

    entt::registry m_registry;

    moss::ComponentRegistry m_componentRegistry;
};

} // moss
