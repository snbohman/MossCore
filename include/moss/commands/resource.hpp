/**
 * @file commands/resource.hpp
 * @brief Resource command: one shared object per type, stored in the registry.
 *
 * Wraps entt::registry::ctx() (EnTT 3.10+). A resource is a singleton
 * keyed by its type. Use it to share state between systems that cannot
 * reference each other, for example a renderer core and its attachments.
 *
 * -   `get`, `find`, `contains` work with a READ or WRITE key.
 * -   `emplace`, `erase` need a WRITE key (build/exit phases).
 *
 * Misuse (missing registry, missing resource on `get`, double `emplace`,
 * writing with a READ key) logs an error and aborts instead of continuing
 * into undefined behaviour.
 *
 * Do not keep the returned reference across frames. Re-fetch it in `tick`;
 * it is a single hash lookup.
 *
 * Example:
 *     // build, WRITE key
 *     auto& core = cmd::Resource<RenderCore>::init(key).emplace();
 *     // anywhere else
 *     auto& core = cmd::Resource<RenderCore>::init(key).get();
 */

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/logs.hpp>
#include <moss/core/key.hpp>
#include <moss/commands/primitives.hpp>

#include <cstdlib>
#include <typeinfo>
#include <utility>

namespace moss::commands {

template<typename T>
class Resource {
public:
    Resource() = default;
    Resource(const Key<key::READ>& key) { apply(key); }
    Resource(const Key<key::WRITE>& key) { apply(key); }

    static Resource init() { return Resource(); }
    static Resource init(const Key<key::READ>& key) { return Resource(key); }
    static Resource init(const Key<key::WRITE>& key) { return Resource(key); }

    void apply(const Key<key::READ>& key) { m_registry = key.m_registry; m_writable = false; }
    void apply(const Key<key::WRITE>& key) { m_registry = key.m_registry; m_writable = true; }
    void clean() { m_registry = nullptr; m_writable = false; }

    /// Constructs the resource in place. WRITE key only. Aborts if it already exists.
    template<typename... Args>
    T& emplace(Args&&... args) {
        requireRegistry();
        requireWritable("emplace");

        if (m_registry->ctx().contains<T>()) {
            M_ERROR("Resource '{}' already exists", typeid(T).name());
            std::abort();
        }

        return m_registry->ctx().emplace<T>(std::forward<Args>(args)...);
    }

    /// Returns the resource. Aborts if it does not exist.
    [[nodiscard]] T& get() {
        requireRegistry();

        T* resource = m_registry->ctx().find<T>();
        if (resource == nullptr) {
            M_ERROR("Resource '{}' does not exist", typeid(T).name());
            std::abort();
        }

        return *resource;
    }

    /// Returns the resource, or nullptr if it does not exist.
    [[nodiscard]] T* find() {
        requireRegistry();
        return m_registry->ctx().find<T>();
    }

    [[nodiscard]] bool contains() const {
        requireRegistry();
        return m_registry->ctx().contains<T>();
    }

    /// Destroys the resource. WRITE key only.
    void erase() {
        requireRegistry();
        requireWritable("erase");
        m_registry->ctx().erase<T>();
    }

private:
    void requireRegistry() const {
        if (m_registry == nullptr) {
            M_ERRORF("Registry is null. Note that apply must be called before any get method");
            std::abort();
        }
    }

    void requireWritable(const char* what) const {
        if (!m_writable) {
            M_ERROR("Resource::{} needs a WRITE key", what);
            std::abort();
        }
    }

    entt::registry* m_registry = nullptr;
    bool m_writable = false;
};

} // moss::commands
