/**
 * @file    core/context.hpp
 * @brief   Defines the `Context` class that groups systems and manages entities.
 *
 * The `Context` class represents a collection of entities and systems.
 * It provides methods for initializing, building, ticking, and exiting
 * the context. Each context is a specialized group of systems that operates
 * on a set of entities. 
 */

#pragma once

#include <moss/ecs/system.hpp>
#include <moss/commands/primitives.hpp>

namespace moss {

class Mirror;

/**
 * @class Context
 * @brief Base class for defining grouped logic (systems + entities).
 *
 * Contexts are created by the user and mounted into the App. They define a
 * self-contained unit of game/application logic. The `init()` method is the only
 * function intended to be overridden, where entity creation and system attachment
 * is performed via the Mirror class.
 *
 * The internal lifecycle (`build`, `tick`, `exit`) is managed by the engine and
 * *can't* be overridden. These methods apply to all systems registered during `init()`.
 */
class Context {
public:
    virtual ~Context() = default;

    virtual void init(Mirror& mirror) { }
    void build(const Key<key::WRITE>& key);
    void tick(const Key<key::READ>& key);
    void exit(const Key<key::WRITE>& key);

private:
    friend class Mirror;
    friend class App;

    DynamicView m_view;
    std::vector<std::unique_ptr<System>> m_systems;
};

}
