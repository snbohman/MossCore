/*
core/app.cpp - parentless

Implements core/app.hpp.

*/

#include <moss/meta/libs.hpp>
#include <moss/core/app.hpp>
#include <moss/core/scene.hpp>
#include <moss/ecs/ecs.hpp>
#include <moss/utils/config.hpp>
#include <memory>


//////////////////////
//// -- Public -- ////
//////////////////////
void moss::App::run() {
    /*while (!raylib::WindowShouldClose()) {*/
        getCurrentScene()->tick({ .registry = getCurrentScene()->registry });
    /*}*/
}

void moss::App::addScene(const char* id, const bool& currentScene) {
    // -- Check for already existing scene -- //
    for (const entt::entity& entity : m_registry.view<Scene>()) {
        if (m_registry.get<std::unique_ptr<Scene>>(entity)->id == id) ERROR("Scene registered with id \"{}\" already exists", id);
    }

    // -- Register Scene -- //
    entt::entity sceneEntity = m_registry.create();
    std::unique_ptr<moss::Scene>& scene = m_registry.emplace<std::unique_ptr<moss::Scene>>(sceneEntity, std::make_unique<moss::Scene>(moss::SceneInitCrate{id, m_componentRegistry}));

    // -- Handle Current Scene-- //
    if (currentScene) {
        auto view = m_registry.view<moss::components::engine::CurrentSceneTag>();
        if (view.size() != 0) { m_registry.remove<moss::components::engine::CurrentSceneTag>(*view.begin()); }
        m_registry.emplace<moss::components::engine::CurrentSceneTag>(sceneEntity);
    }
}

void moss::App::setCurrentScene(const char* id) {
    for (const entt::entity& entity : m_registry.view<Scene>()) {
        if (m_registry.get<std::unique_ptr<Scene>>(entity)->id == id) {
            m_registry.remove<moss::components::engine::CurrentSceneTag>(*m_registry.view<moss::components::engine::CurrentSceneTag>().begin());
            m_registry.emplace<moss::components::engine::CurrentSceneTag>(entity);
            return;
        }
    }

    ERROR("Scene registered with id \"{}\" not found", id);
}

void moss::App::setComponentRegistry(const moss::types::ComponentRegistry& componentRegistry) {
    m_componentRegistry = std::move(componentRegistry);
}

void moss::App::buildComponentRegistry(moss::types::ComponentRegistry& componentRegistry) {
    componentRegistry = {
        FILL_COMPONENT_DATA(components::Transform),
        FILL_COMPONENT_DATA(components::Physics),
        FILL_COMPONENT_DATA(components::Material)
    };
}


///////////////////////
//// -- Private -- ////
///////////////////////
std::unique_ptr<moss::Scene>& moss::App::getScene(const char* id) {
    for (const entt::entity& e : m_registry.view<Scene>()) {
        std::unique_ptr<Scene>& scene = m_registry.get<std::unique_ptr<Scene>>(e);
        if (scene->id == id) { return scene; }
    }

    ERROR("Scene registered with id \"{}\" not found", id);
}

std::unique_ptr<moss::Scene>& moss::App::getCurrentScene() {
    return m_registry.get<std::unique_ptr<Scene>>(*m_registry.view<components::engine::CurrentSceneTag>().begin());
}


////////////////////////
//// -- Builders -- ////
////////////////////////
moss::App::~App() { }
moss::App::App() {
    json renderConfig;
    utils::config::readConfig(renderConfig, "data/renderConfig.json");

    /* -- Register Renderer -- */
    for (const auto& [rendererName, active] : renderConfig["renderers"].items()) {
        auto it = m_renderRegistry.find(rendererName);
        WARN_IF(it == m_componentRegistry.end(), "Renderer \"{}\" not found in componentRegister", rendererName);

        if (!active) continue;

        it.second(m_renderer, )
    }
}
