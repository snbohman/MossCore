/*
core/app.cpp - parentless

Implements core/app.hpp.

*/

#include <moss/includes.hpp>
#include <moss/core/app.hpp>
#include <moss/core/scene.hpp>
#include <moss/ecs/components.hpp>
#include <moss/ecs/renderable.hpp>
#include <fstream>
#include <memory>


//////////////////////
//// -- Public -- ////
//////////////////////
void moss::App::run() {
    while (!raylib::WindowShouldClose()) {
        getCurrentScene()->tick({ .registry = getCurrentScene()->registry });
    }
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
        auto view = m_registry.view<moss::CurrentSceneTag>();
        if (view.size() != 0) { m_registry.remove<moss::CurrentSceneTag>(*view.begin()); }
        m_registry.emplace<moss::CurrentSceneTag>(sceneEntity);
    }
}

void moss::App::setCurrentScene(const char* id) {
    for (const entt::entity& entity : m_registry.view<Scene>()) {
        if (m_registry.get<std::unique_ptr<Scene>>(entity)->id == id) {
            m_registry.remove<moss::CurrentSceneTag>(*m_registry.view<moss::CurrentSceneTag>().begin());
            m_registry.emplace<moss::CurrentSceneTag>(entity);
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
        FILL_COMPONENT_DATA(moss::RectTransform),
        FILL_COMPONENT_DATA(moss::CircleTransform),
        FILL_COMPONENT_DATA(moss::PolyTransform),
        FILL_COMPONENT_DATA(moss::RegPolyTransform),
        FILL_COMPONENT_DATA(moss::Physics),
        FILL_COMPONENT_DATA(moss::Material),
        FILL_RENDERABLE_DATA(moss::RectRenderable),
        FILL_RENDERABLE_DATA(moss::CircleRenderable),
        FILL_RENDERABLE_DATA(moss::PolyRenderable),
        FILL_RENDERABLE_DATA(moss::RegPolyRenderable),
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
    return m_registry.get<std::unique_ptr<Scene>>(*m_registry.view<CurrentSceneTag>().begin());
}


////////////////////////
//// -- Builders -- ////
////////////////////////
moss::App::~App() { raylib::CloseWindow(); }
moss::App::App() {
    // -- Load Window Config -- //
    json windowConfig = json::parse(std::ifstream("data/renderConfig.json"))["window"];
    ERROR_IF(windowConfig.empty(), "WindowConfig found empty");

    // -- Initialize Raylib -- //
    raylib::SetTraceLogLevel(raylib::LOG_WARNING);
    raylib::InitWindow(
        windowConfig["windowDimensions"][0].get<glm::u16>(),
        windowConfig["windowDimensions"][1].get<glm::u16>(),
        windowConfig["title"].get<std::string>().c_str()
    );
    raylib::SetTargetFPS(windowConfig["targetFPS"].get<glm::u16>());
}
