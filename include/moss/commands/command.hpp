/**
 * @file    commands/command.hpp
 * @brief   Base of every command: holds the registry and makes `init(key)`.
 *
 * A command is a small handle (a few pointers), cheap to create and to copy.
 * The usual form is a temporary:
 *
 *     auto rows = cmd::Query<With<Foo>>::init(key).atlas();
 *
 * or one kept for several calls in the same function:
 *
 *     auto foos = cmd::Query<With<Foo>>::init(key);
 *
 * Both READ and WRITE keys are accepted everywhere. A command used before
 * apply(key) aborts with a message (it does not continue into undefined
 * behaviour).
 */

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/logs.hpp>
#include <moss/core/key.hpp>
#include <moss/commands/primitives.hpp>

namespace moss::commands {

template<typename Derived>
class Command {
public:
    template<key::Permissions P>
    [[nodiscard]] static Derived init(const Key<P>& key) {
        Derived command;
        command.apply(key);
        return command;
    }

    // Tests and tools that hold a registry directly. Treated as a WRITE key.
    // `quit` is only needed by cmd::Quit.
    [[nodiscard]] static Derived init(entt::registry* registry, bool* quit = nullptr) {
        Derived command;
        command.apply(registry, quit);
        return command;
    }

    void apply(entt::registry* registry, bool* quit = nullptr) {
        m_registry = registry;
        m_quit = quit;
        m_writable = true;
    }

    template<key::Permissions P>
    void apply(const Key<P>& key) {
        m_registry = detail::KeyAccess::registry(key);
        m_quit = detail::KeyAccess::quit(key);
        m_writable = (P == moss::key::WRITE);
    }

    [[nodiscard]] bool bound() const { return m_registry != nullptr; }

protected:
    Command() = default;
    ~Command() = default;

    [[nodiscard]] entt::registry& registry() const {
        M_FATAL_IFF(m_registry == nullptr, "Command used before apply(key): the registry is null");
        return *m_registry;
    }

    [[nodiscard]] bool& quitFlag() const {
        M_FATAL_IFF(m_quit == nullptr, "Command used before apply(key): the quit flag is null");
        return *m_quit;
    }

    void requireWritable(const char* what) const {
        M_FATAL_IF(!m_writable, "{} needs a WRITE key (build or exit phase)", what);
    }

private:
    entt::registry* m_registry = nullptr;
    bool* m_quit = nullptr;
    bool m_writable = false;
};

}
