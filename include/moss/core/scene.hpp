/*

Handles the ecs of the scene. Has a unique
registry, only including entities of the scene.

Id is a char that the scene holds.

Is owned by App. The current scene has a tag
applied. The Scene class is techincally a system
attatched to an entity. The scene includes an id
set by the user.

*/


#pragma once

#include <moss/meta/libs.hpp>
#include <moss/ecs/ecs.hpp>
#include <moss/meta/defines.hpp>


namespace moss {

namespace scene {

struct InitCrate {
    const char* id;
    const types::AttachmentRegistry& componentRegistry;
};

}

class Scene : public System {
public:
    // -- Special init for scene system -- //
    // -- Therefor a builder -- //
    Scene(scene::InitCrate crate);
    void tick() override;

    const char* id;
    entt::registry registry;
};

} // moss
