/*
 * @file    core/key.hpp
 * @brief   Singleton Contex class providing access to the ECS registry.
 *
 * This class provides a singleton instance for managing ECS operations, ensuring
 * that only one instance exists to prevent conflicting permission mappings.
 * It offers two primary modes of interaction:
 *
 * 1.  **Compile-Time Safe Commands:** Utilized through command structures, offering
 *     compile-time safety and type checking for ECS operations.
 *
 * 2.  **Runtime Fluent API:** A fluent design pattern (inspired by rs-bevvy) for
 *     runtime entity creation and component attachment. This approach, while
 *     sacrificing some compile-time safety, provides a more flexible and
 *     ergonomic API for rapid prototyping or less critical tasks.  The runtime
 *     API is accessed through the `Contex` functions (e.g., `create()`, `attach()`).
 *
 * The class uses mutex locks to guarantee thread-safe access and prevent
 * multiple instances from being created.
 *
 * **Usage Examples (Runtime API):**
 *
 * ```cpp
 * Contex::init()
 *     .create()
 *         .attach<PlayerTag, PlayerMovement, Position>()
 *     .create(10)
 *         .attach<EnemyTag, EnemyMovement, Position>();
 * ```
 *
 * **Singleton Management:**
 *
 * ```cpp
 * Contex::get();     // THROWS RUNTIME ERROR if not initialized
 * Contex::init();
 * Contex::get();     // Runs successfully
 * Contex::destroy();
 * Contex::get();     // THROWS RUNTIME ERROR after destruction
 * ```
 */


#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <moss/commands/primitives.hpp>

class C;

namespace moss {

namespace key {

enum Permissions {
    READ = 1 << 0,
    WRITE = 1 << 1
};

}

template<key::Permissions P>
class Key {
public:
    Key<P>(const Key<P>&) = delete;
    Key<P>& operator=(const Key<P>&) = delete;
    Key<P>(Key<P>&&) = delete;
    Key<P>& operator=(Key<P>&&) = delete;
 
private:
    friend class Contex;
    template<typename Wth, typename Vw> friend class commands::Query;
    template<typename Wth> friend class commands::DynamicQuery;
    template<typename Inc, typename Ex> friend class commands::View;

    Key() = default;
    ~Key() = default;

    entt::registry* m_registry;
};

}
