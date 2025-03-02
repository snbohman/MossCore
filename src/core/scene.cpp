#include <moss/includes.hpp>
#include <moss/core/scene.hpp>
#include <moss/types.hpp>
#include <fstream>


moss::Scene::~Scene() { }
moss::Scene::Scene(const char* _id, const moss::ComponentRegistry& componentRegistry) : id(_id) {
    json sceneConfig = json::parse(std::ifstream("data/sceneConfig.json"))[id];

    for (const auto& [eName, data] : sceneConfig["entities"].items()) {
        entt::entity entity = registry.create();

        for (const auto& [cName, cData] : data.items()) {
            auto it = componentRegistry.find(cName);
            if (it == componentRegistry.end()) { spdlog::error("In moss::Scene::Scene(): Component/Tag/System \"{cName}\" not registered."); std::abort(); };
            it->second(entity, cData);
        }
    }
}

void moss::Scene::tick() {
    for (const auto& entity : registry.view<System>()) {
        registry.get<std::unique_ptr<System>>(entity)->tick();
        registry.get<std::unique_ptr<System>>(entity)->tick(registry);
        registry.get<std::unique_ptr<System>>(entity)->tick(registry, raylib::GetFrameTime());
    }
}
