/*
core/scene.cpp - App

Implements core/scene.hpp

*/

#include <moss/includes.hpp>
#include <moss/core/scene.hpp>
#include <moss/defines.hpp>
#include <fstream>


//////////////////////
//// -- Public -- ////
//////////////////////
void moss::Scene::tick() {
    // -- Run Systems -- //
    for (const auto& entity : registry.view<std::unique_ptr<System>>()) {
        registry.get<std::unique_ptr<System>>(entity)->tick();
        registry.get<std::unique_ptr<System>>(entity)->tick(registry);
        registry.get<std::unique_ptr<System>>(entity)->tick(registry, raylib::GetFrameTime());
    }
}


////////////////////////
//// -- Builders -- ////
////////////////////////
moss::Scene::Scene(const char* _id, const moss::types::ComponentRegistry& componentRegistry) : id(_id) {
    // -- Load Scene Config -- //
    json sceneConfig = json::parse(std::ifstream("data/sceneConfig.json"))[id];
    ERROR_IF(sceneConfig.empty(), "SceneConfig found empty");

    // -- Register Components Into Registry -- //
    for (const auto& [eName, data] : sceneConfig["entities"].items()) {
        entt::entity entity = registry.create();
        for (const auto& [cName, cData] : data.items()) {
            auto it = componentRegistry.find(cName);
            WARN_IF(it == componentRegistry.end(), "Entity attatchment (Component/Tag/System) \"{}\" not registered", cName)
            if (it != componentRegistry.end()) it->second(registry, entity, cData);
        }
    }
}
