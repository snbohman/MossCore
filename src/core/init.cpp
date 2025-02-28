#include <moss/includes.hpp>
#include <moss/core/init.hpp>
#include <moss/components/serialization.hpp>
#include <moss/components/components.hpp>


void moss::initRaylib(const json& gameConfig) {
    raylib::SetTraceLogLevel(raylib::LOG_WARNING);
    raylib::InitWindow(
        gameConfig["core"]["windowDimensions"][0].get<glm::u16>(),
        gameConfig["core"]["windowDimensions"][1].get<glm::u16>(),
        gameConfig["core"]["title"].get<std::string>().c_str()
    );
    raylib::SetTargetFPS(gameConfig["core"]["targetFPS"].get<glm::u16>());
}

void moss::initEntt(const json& gameConfig, entt::registry& registry, const moss::ComponentRegistry& componentRegistry) {
    for (const auto& [eName, components] : gameConfig["entities"].items()) {
        entt::entity entity = registry.create();

        for (const auto& [cName, cData] : components.items()) {
            auto it = componentRegistry.find(cName);
            if (it == componentRegistry.end()) continue;
            it->second(entity, cData);
        }
    }
}

void moss::buildComponentRegistry(entt::registry& registry, moss::ComponentRegistry& componentMap) {
    componentMap = {
        FILL_COMPONENT_DATA(moss::RectTransform),
        FILL_COMPONENT_DATA(moss::CircleTransform),
        FILL_COMPONENT_DATA(moss::PolyTransform),
        FILL_COMPONENT_DATA(moss::RegPolyTransform),
        FILL_COMPONENT_DATA(moss::Physics),
        FILL_COMPONENT_DATA(moss::Material)
    };
}
