/*
core/app.cpp - parentless

Implements core/app.hpp.

*/

#include <moss/includes.hpp>
#include <moss/core/app.hpp>
#include <moss/core/scene.hpp>
#include <moss/core/render.hpp>
#include <moss/ecs/components.hpp>
#include <fstream>
#include <memory>


//////////////////////
//// -- Public -- ////
//////////////////////
void moss::App::run() {
    while (!raylib::WindowShouldClose()) {
        getCurrentScene()->tick();
        getRender()->tick(getCurrentScene()->registry);
    }
}

void moss::App::addScene(const char* id, const bool& currentScene) {
    // -- Register Scene -- //
    entt::entity sceneEntity = m_registry.create();
    std::unique_ptr<moss::Scene>& scene = m_registry.emplace<std::unique_ptr<moss::Scene>>(sceneEntity, std::make_unique<moss::Scene>(id, m_componentRegistry));

    // -- Handle Current Scene-- //
    if (currentScene) {
        auto view = m_registry.view<CurrentSceneTag>();
        if (view.size() != 0) { m_registry.remove<CurrentSceneTag>(*view.begin()); }
        m_registry.emplace<CurrentSceneTag>(sceneEntity);
    }
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
        FILL_COMPONENT_DATA(moss::Material)
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

std::unique_ptr<moss::Render>& moss::App::getRender() {
    return m_registry.get<std::unique_ptr<Render>>(*m_registry.view<std::unique_ptr<Render>>().begin());
}


////////////////////////
//// -- Builders -- ////
////////////////////////
moss::App::~App() { raylib::CloseWindow(); }
moss::App::App() {
    // -- Load Window Config -- //
    json windowConfig = json::parse(std::ifstream("data/projectConfig.json"))["windowConfig"];
    ERROR_IF(windowConfig.empty(), "WindowConfig found empty");

    // -- Initialize Raylib -- //
    raylib::SetTraceLogLevel(raylib::LOG_WARNING);
    raylib::InitWindow(
        windowConfig["windowDimensions"][0].get<glm::u16>(),
        windowConfig["windowDimensions"][1].get<glm::u16>(),
        windowConfig["title"].get<std::string>().c_str()
    );
    raylib::SetTargetFPS(windowConfig["targetFPS"].get<glm::u16>());

    // -- Initialize Render System -- //
    entt::entity renderEntity = m_registry.create();
    m_registry.emplace<std::unique_ptr<moss::Render>>(renderEntity, std::make_unique<moss::Render>());
}


