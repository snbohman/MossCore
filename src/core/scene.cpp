#include <moss/meta/libs.hpp>
#include <moss/utils/config.hpp>
#include <moss/core/scene.hpp>


//////////////////////
//// -- Public -- ////
//////////////////////
void moss::Scene::tick() {
    // -- Run Systems -- //
    for (const entt::entity& entity : registry.view<std::unique_ptr<System>>()) {
        registry.get<std::unique_ptr<System>>(entity)->tick({ .entity = entity, .registry = registry });
    }
}


////////////////////////
//// -- Builders -- ////
////////////////////////
moss::Scene::Scene(scene::InitCrate crate) : id(crate.id) {
    // -- Load Scene Config -- //
    json sceneConfig;
    utils::config::readConfig(sceneConfig, "sceneConfig.json");

    // -- Register Components Into Registry -- //
    for (const auto& [eName, data] : sceneConfig[id]["entities"].items()) {
        entt::entity entity = registry.create();
        for (const auto& [cName, cData] : data.items()) {
            auto it = crate.componentRegistry.find(cName);
            WARN_IF(it == crate.componentRegistry.end(), "Entity attatchment (Component/Tag/System) \"{}\" not registered", cName);

            if (it != crate.componentRegistry.end()) it->second(registry, entity, cData);
        }
    }
}
