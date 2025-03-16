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
#include <moss/meta/defines.hpp>
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
    INFO_INIT("Initializing scene \"{}\"", 3, id);

    // -- Check for already existing scene -- //
    for (const entt::entity& entity : m_registry.view<Scene>()) {
        if (m_registry.get<std::unique_ptr<Scene>>(entity)->id == id) ERROR("Scene registered with id \"{}\" already exists", id);
    }

    // -- Register Scene -- //
    entt::entity sceneEntity = m_registry.create();
    std::unique_ptr<Scene>& scene = m_registry.emplace<std::unique_ptr<Scene>>(sceneEntity, std::make_unique<Scene>(scene::InitCrate{id, m_attachmentRegistry}));

    // -- Handle Current Scene-- //
    if (currentScene) {
        auto view = m_registry.view<cmp::engine::CurrentSceneTag>();
        if (view.size() != 0) { m_registry.remove<cmp::engine::CurrentSceneTag>(*view.begin()); }
        m_registry.emplace<cmp::engine::CurrentSceneTag>(sceneEntity);
    }
}

void moss::App::setCurrentScene(const char* id) {
    INFO_INIT("Making scene \"{}\" current", 3, id);

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
    INFO_INITF("Moving attatchment registry", 3);
    m_attachmentRegistry = std::move(attachmentRegistry);
}

void moss::App::buildAttachmentRegistry(types::AttachmentRegistry& attachmentRegistry) {
    INFO_INITF("Building attatchment registry", 3);
    attachmentRegistry = {
        FILL_COMPONENT_DATA(moss::components::Transform),
        FILL_COMPONENT_DATA(moss::components::RigidBody),
        FILL_COMPONENT_DATA(moss::components::RectCollider),
        FILL_COMPONENT_DATA(moss::components::Material),
        FILL_RENDERABLE_DATA(moss::renderables::Circle),
        FILL_RENDERER_DATA(moss::render::MRLS),

        FILL_COMPONENT_DATA(moss::cmp::Transform),
        FILL_COMPONENT_DATA(moss::cmp::RigidBody),
        FILL_COMPONENT_DATA(moss::cmp::RectCollider),
        FILL_COMPONENT_DATA(moss::cmp::Material),
        FILL_RENDERABLE_DATA(moss::rcmp::Circle),
        FILL_RENDERER_DATA(moss::rnd::MRLS),

        FILL_COMPONENT_DATA(cmp::Transform),
        FILL_COMPONENT_DATA(cmp::RigidBody),
        FILL_COMPONENT_DATA(cmp::RectCollider),
        FILL_COMPONENT_DATA(cmp::Material),
        FILL_RENDERABLE_DATA(rcmp::Circle),
        FILL_RENDERER_DATA(rnd::MRLS)
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


////////////////////
//// -- Init -- ////
////////////////////
void moss::App::init(app::InitCrate crate) {
    INFO_INITF("Reading renderConfig", 1);

    json renderConfig;
    utils::config::readConfig(renderConfig, "renderConfig.json", crate.dataDirectory);

    // -- Register Renderer -- //
    INFO_INITF("Initializing renderer", 2);
    entt::entity renderEntity = m_registry.create();
    for (const auto& [rendererName, active] : renderConfig["renderers"].items()) {
        auto it = m_attachmentRegistry.find(rendererName);
        WARN_IF(it == m_attachmentRegistry.end(), "Renderer \"{}\" not found in attachmentRegistry", rendererName);

        if (active) {
            INFO_INIT("Renderer \"{}\" initialized", 3, rendererName);
            it->second(m_registry, renderEntity, {});
            m_renderer = m_registry.get<std::unique_ptr<Renderer>>(renderEntity).get();
            break;
        }
    }
}
