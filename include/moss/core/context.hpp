/**
 * @file    core/context.hpp
 * @brief   Context: a group of systems plus the entities they work on.
 *
 * A Context is the unit of logic you write: override init(), and describe the
 * entities and systems through the Mirror:
 *
 *     class MyContext : public Context {
 *     public:
 *         void init(Mirror& mirror) override {
 *             mirror.connect<SystemA, SystemB>()
 *                   .create().attach<Foo>({.x = 1});
 *         }
 *     };
 *
 * Every entity created through the Mirror belongs to the Context, and every
 * system of the Context receives that list as the `entities` argument.
 * The lifecycle (build, tick, exit) belongs to the App and cannot be
 * overridden: build and tick run systems in connection order, exit in reverse.
 */

#pragma once

#include <moss/ecs/system.hpp>
#include <moss/commands/primitives.hpp>

#include <memory>
#include <vector>

namespace moss {

class Mirror;
class App;

class Context {
public:
    virtual ~Context() = default;

    virtual void init(Mirror&) { }

private:
    friend class Mirror;
    friend class App;

    void build(const Key<key::WRITE>& key) {
        for (auto& system : m_systems) {
            system->build();
            system->build(key);
            system->build(key, m_view);
        }
    }

    void tick(const Key<key::READ>& key) {
        for (auto& system : m_systems) {
            system->tick();
            system->tick(key);
            system->tick(key, m_view);
        }
    }

    // Reverse order: a system is torn down before the ones it depends on
    void exit(const Key<key::WRITE>& key) {
        for (auto it = m_systems.rbegin(); it != m_systems.rend(); ++it) {
            System& system = **it;
            system.exit();
            system.exit(key);
            system.exit(key, m_view);
        }
    }

    DynamicView m_view;
    std::vector<std::unique_ptr<System>> m_systems;
};

}
