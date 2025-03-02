#include "moss/ecs/defaultComponents.hpp"
#include <memory>
#include <moss/includes.hpp>
#include <moss/core/app.hpp>
#include <moss/core/scene.hpp>
#include <moss/core/render.hpp>
#include <moss/ecs/components.hpp>
#include <fstream>


moss::App::~App() { }
moss::App::App() {
    json windowConfig = json::parse(std::ifstream("data/projectConfig.json"))["windowConfig"];

    raylib::SetTraceLogLevel(raylib::LOG_WARNING);
    raylib::InitWindow(
        windowConfig["windowDimensions"][0].get<glm::u16>(),
        windowConfig["windowDimensions"][1].get<glm::u16>(),
        windowConfig["title"].get<std::string>().c_str()
    );
    raylib::SetTargetFPS(windowConfig["targetFPS"].get<glm::u16>());

    entt::entity renderEntity = m_registry.create();
    m_registry.emplace<std::unique_ptr<System>>(renderEntity, std::make_unique<Render>());
}

void moss::App::run() {
    while (!raylib::WindowShouldClose()) {
        getCurrentScene()->tick();
        getRender()->tick(getCurrentScene()->registry);
    }
}

void moss::App::addScene(const char* id, const bool& currentScene) {
    entt::entity sceneEntity = m_registry.create();
    auto& scene = m_registry.emplace<std::unique_ptr<System>>(sceneEntity, std::make_unique<Scene>(id, m_componentRegistry));

    if (!currentScene) return;

    auto view = m_registry.view<CurrentSceneTag>();
    if (view.size() != 0) { m_registry.remove<CurrentSceneTag>(*view.begin()); }
    m_registry.emplace<CurrentSceneTag>(sceneEntity);
}

std::unique_ptr<moss::Scene>& moss::App::getScene(const char* id) {
    for (const entt::entity& e : m_registry.view<Scene>()) {
        std::unique_ptr<Scene>& scene = m_registry.get<std::unique_ptr<Scene>>(e);
        if (scene->id == id) { return scene; }
    }

    spdlog::error("In moss::App::getScene: Scene \"{}\" not found."); std::abort();
}

std::unique_ptr<moss::Scene>& moss::App::getCurrentScene() {
    return m_registry.get<std::unique_ptr<Scene>>(*m_registry.view<CurrentSceneTag>().begin());
}

std::unique_ptr<moss::Render>& moss::App::getRender() {
    return m_registry.get<std::unique_ptr<Render>>(*m_registry.view<Render>().begin());
}

void moss::App::buildComponentRegistry(entt::registry& registry, moss::ComponentRegistry& componentRegistry) {
    componentRegistry = {
        FILL_COMPONENT_DATA(moss::RectTransform),
        FILL_COMPONENT_DATA(moss::CircleTransform),
        FILL_COMPONENT_DATA(moss::PolyTransform),
        FILL_COMPONENT_DATA(moss::RegPolyTransform),
        FILL_COMPONENT_DATA(moss::Physics),
        FILL_COMPONENT_DATA(moss::Material)
    };
}

void moss::App::setComponentRegistry(moss::ComponentRegistry& componentRegistry) {
    m_componentRegistry = componentRegistry;
}
