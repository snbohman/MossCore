/**
 * @file    commands/destroy.hpp
 * @brief   Destroy: remove entities (and all their components).
 *
 *     cmd::Destroy::init(key).one(e);
 *     cmd::Destroy::init(key).many(list);
 *
 * An entity that is already gone is skipped, not an error. Entity lists kept
 * elsewhere (a Context's view, your own DynamicView) are not updated:
 * Query::atlas(subset) skips dead entities, other code must check for itself.
 */

#pragma once

#include <moss/commands/command.hpp>

namespace moss::commands {

class Destroy : public Command<Destroy> {
public:
    Destroy() = default;

    // false when the entity was already gone
    bool one(Entity entity) const {
        entt::registry& reg = registry();
        if (!reg.valid(entity)) return false;

        reg.destroy(entity);
        return true;
    }

    // Returns how many entities were actually destroyed
    usize many(const DynamicView& entities) const {
        usize destroyed = 0;
        for (Entity entity : entities)
            if (one(entity)) destroyed++;
        return destroyed;
    }
};

}
