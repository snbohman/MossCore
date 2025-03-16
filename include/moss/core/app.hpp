/*
core/app.hpp - parentless

Core application.

*/

#pragma once

#include <moss/core/scene.hpp>
#include <moss/render/render.hpp>
#include <moss/meta/defines.hpp>
#include <memory>


namespace moss {

namespace app {

struct InitCrate {
    const char* dataDirectory = "data";
};

} // app

class App {
public:
    inline App() { spdlog::set_pattern("[\e[1;32mMOSS\e[0m] [%^%L%$] %v"); };
    inline ~App() { };

    void init(app::InitCrate crate);
    void run();

    void addScene(const char* id, const bool& currentScene);
    void buildAttachmentRegistry(types::AttachmentRegistry& componentRegistry);
    void setAttachmentRegistry(const types::AttachmentRegistry& componentRegistry);
    void setCurrentScene(const char* id);

private:
    // -- These are only used by app (eventhough they seem public) -- //
    // -- The components should have no access to these -- //
    std::unique_ptr<moss::Scene>& getScene(const char* id);
    std::unique_ptr<moss::Scene>& getCurrentScene();

    entt::registry m_registry;
    moss::types::AttachmentRegistry m_attachmentRegistry;
    Renderer* m_renderer;
};

} // moss
