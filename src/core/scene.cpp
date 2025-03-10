/*
core/scene.cpp - App

Implements core/scene.hpp

*/

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <moss/core/scene.hpp>
#include <fstream>
#include <raylib.h>


//////////////////////
//// -- Public -- ////
//////////////////////
void moss::Scene::tick(moss::SystemTickCrate crate) {
    // -- Run Systems -- //
    for (const entt::entity& entity : registry.view<std::unique_ptr<moss::System>>()) {
        registry.get<std::unique_ptr<moss::System>>(entity)->tick({.entity = entity, .registry = registry, .deltaTime = raylib::GetFrameTime()});
    }

    raylib::BeginDrawing();
    raylib::ClearBackground(raylib::BLACK);

    for (const entt::entity& entity : registry.view<std::unique_ptr<moss::Renderable>>()) {
        registry.get<std::unique_ptr<moss::Renderable>>(entity)->render({ .entity = entity, .registry = registry });
    }

    raylib::EndDrawing();
}


////////////////////////
//// -- Builders -- ////
////////////////////////
moss::Scene::Scene(moss::SceneInitCrate crate) : id(crate.id) {
    // -- Load Scene Config -- //
    json sceneConfig = json::parse(std::ifstream("data/sceneConfig.json"))[id];
    ERROR_IF(sceneConfig.empty(), "SceneConfig found empty");

    // -- Register Components Into Registry -- //
    for (const auto& [eName, data] : sceneConfig["entities"].items()) {
        entt::entity entity = registry.create();
        for (const auto& [cName, cData] : data.items()) {
            auto it = crate.componentRegistry.find(cName);
            WARN_IF(it == crate.componentRegistry.end(), "Entity attatchment (Component/Tag/System) \"{}\" not registered", cName)
            if (it != crate.componentRegistry.end()) it->second(registry, entity, cData);
        }
    }
}
