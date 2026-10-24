/**
 * @file    core/key.hpp
 * @brief   Key: the token every command is created from.
 *
 * The engine hands a Key<WRITE> to build() and exit() and a Key<READ> to tick().
 * Commands are made from one with `Command::init(key)`; without a key nothing
 * can reach the registry. READ and WRITE do not restrict components, they name
 * the phase: operations that only make sense while building or tearing down
 * (cmd::Resource::emplace / erase) require WRITE.
 *
 * Keys cannot be copied or moved and only App can make them. Commands read the
 * registry through detail::KeyAccess, which is internal.
 */

#pragma once

#include <moss/meta/libs.hpp>

namespace moss {

namespace key {

enum Permissions {
    READ,
    WRITE
};

}

namespace detail { struct KeyAccess; }

template<key::Permissions P>
class Key {
public:
    static constexpr key::Permissions permission = P;

    Key(const Key&) = delete;
    Key& operator=(const Key&) = delete;
    Key(Key&&) = delete;
    Key& operator=(Key&&) = delete;

private:
    friend class App;
    friend struct detail::KeyAccess;

    Key(entt::registry* registry, bool* quit) : m_registry(registry), m_quit(quit) { }

    entt::registry* m_registry = nullptr;
    bool* m_quit = nullptr;
};

namespace detail {

// The only door into a Key. Internal: used by Command.
struct KeyAccess {
    template<key::Permissions P>
    static entt::registry* registry(const Key<P>& key) { return key.m_registry; }

    template<key::Permissions P>
    static bool* quit(const Key<P>& key) { return key.m_quit; }
};

}

}
