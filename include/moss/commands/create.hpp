/**
 * @file    commands/create.hpp
 * @brief   Create: new entities.
 *
 *     Entity e = cmd::Create::init(key).one();
 *     DynamicView es = cmd::Create::init(key).many(10);
 */

#pragma once

#include <moss/commands/command.hpp>

namespace moss::commands {

class Create : public Command<Create> {
public:
    Create() = default;

    [[nodiscard]] Entity one() const { return registry().create(); }

    [[nodiscard]] DynamicView many(u32 count) const {
        entt::registry& reg = registry();

        DynamicView out;
        out.reserve(count);
        for (u32 i = 0; i < count; i++) out.push_back(reg.create());
        return out;
    }
};

}
