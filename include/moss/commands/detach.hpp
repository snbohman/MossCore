/**
 * @file    commands/detach.hpp
 * @brief   Detach: remove components from entities (the entities stay).
 *
 *     cmd::Detach<With<Foo, Tag>>::init(key).from(entity);
 *     cmd::Detach<With<Foo>>::init(key).from(entities);
 *
 * Components an entity does not have are skipped. Returns how many components
 * were removed.
 */

#pragma once

#include <moss/commands/command.hpp>

namespace moss::commands {

template<typename... C>
requires(sizeof...(C) > 0)
class Detach<With<C...>> : public Command<Detach<With<C...>>> {
public:
    Detach() = default;

    usize from(Entity entity) const {
        entt::registry& reg = this->registry();
        if (!reg.valid(entity)) return 0;
        return reg.remove<C...>(entity);
    }

    usize from(const DynamicView& entities) const {
        usize removed = 0;
        for (Entity entity : entities) removed += from(entity);
        return removed;
    }
};

}
