/*
core/app.cpp - parentless

Implements core/app.hpp.

*/

#include <moss/meta/libs.hpp>
#include <moss/core/app.hpp>
#include <moss/core/scene.hpp>
#include <moss/render/render.hpp>
#include <moss/ecs/ecs.hpp>
#include <moss/utils/config.hpp>
#include <memory>


//////////////////////
//// -- Public -- ////
//////////////////////
void moss::App::run() {
    while (!m_renderer->shouldClose()) {
        getCurrentScene()->tick();
        m_renderer->tick({ .registry = getCurrentScene()->registry });
    }
}

void moss::App::addScene(const char* id, const bool& currentScene) {
    // -- Check for already existing scene -- //
    for (const entt::entity& entity : m_registry.view<Scene>()) {
        if (m_registry.get<std::unique_ptr<Scene>>(entity)->id == id) ERROR("Scene registered with id \"{}\" already exists", id);
    }

    // -- Register Scene -- //
    entt::entity sceneEntity = m_registry.create();
    std::unique_ptr<Scene>& scene = m_registry.emplace<std::unique_ptr<Scene>>(sceneEntity, std::make_unique<Scene>(scene::InitCrate{id, m_attachmentRegistry}));

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

void moss::App::setAttachmentRegistry(const types::AttachmentRegistry& attachmentRegistry) {
    m_attachmentRegistry = std::move(attachmentRegistry);
}

void moss::App::buildAttachmentRegistry(types::AttachmentRegistry& attachmentRegistry) {
    attachmentRegistry = {
        FILL_COMPONENT_DATA(cmp::Transform),
        FILL_COMPONENT_DATA(cmp::RigidBody),
        FILL_COMPONENT_DATA(cmp::RectCollider),
        FILL_COMPONENT_DATA(cmp::Material),
        FILL_RENDERABLE_DATA(rnd::Circle),
        FILL_RENDERER_DATA(render::MRLS),
        /*FILL_RENDERER_DATA(render::Vulkan)*/
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
moss::App::App(app::InitCrate crate) {
    json renderConfig;
    utils::config::readConfig(renderConfig, "renderConfig.json", crate.dataDirectory);

    // -- Register Renderer -- //
    entt::entity renderEntity;
    for (const auto& [rendererName, active] : renderConfig["renderers"].items()) {
        auto it = m_attachmentRegistry.find(rendererName);
        WARN_IF(it == m_attachmentRegistry.end(), "Renderer \"{}\" not found in attachmentRegistry", rendererName);

        if (active) {
            it->second(m_registry, renderEntity, { });
            m_renderer = m_registry.get<std::shared_ptr<Renderer>>(renderEntity);
            break;
        }
    }
}
