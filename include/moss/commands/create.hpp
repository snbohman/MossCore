/**
 * @file commands/create.hpp
 * @brief Create command specification
 *
 * Simple create command used by systems.
 *
 */

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/logs.hpp>
#include <moss/meta/types.hpp>
#include <moss/core/key.hpp>
#include <moss/commands/primitives.hpp>

namespace moss::commands {

template<u32 N>
class Create {
public:
    M_SA(N > 0, "Number of entities must be non-negative non-zero integer");

    Create() = default;
    Create(const Key<key::READ>& key) { apply(key); }
    Create(const Key<key::WRITE>& key) { apply(key); }

    static Create init() { return Create(); }
    static Create init(const Key<key::READ>& key) { return Create(key); }
    static Create init(const Key<key::WRITE>& key) { return Create(key); }

    void apply(Key<key::READ> key) { m_registry = key.m_registry; }
    void apply(Key<key::WRITE> key) { m_registry = key.m_registry; }
    void clean() { m_registry = nullptr; }

    [[nodiscard]] DynamicView create(bool doClean = false) {
        M_ERROR_IFF(m_registry == nullptr,
            "Registry is null. Note that apply must be called before any get method"
        );
 
        DynamicView v;
        for (u32 i = 0; i < N; i++) {
            v.push_back(m_registry->create());
        }

        return std::move(v);
    }

private:
    entt::registry* m_registry;
};

} // moss::commands
