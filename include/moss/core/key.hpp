/**
 * @file    key.hpp
 * @brief   Defines the `Key` class and permission system for ECS commands.
 *
 * The `Key` class represents a permission system for commands in the ECS.
 * It ensures that systems and commands only read or write to components 
 * when permitted. The permissions are defined through the `Permissions` 
 * enum and are used to control access to the ECS registry.
 */


#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/defines.hpp>
#include <moss/commands/primitives.hpp>

class C;

namespace moss {

namespace key {

    /**
     * @enum Permissions
     * @brief Defines permissions for command access to components.
     *
     * The `Permissions` enum provides two levels of access: `READ` and `WRITE`.
     * These control the ability to read or modify components in the ECS registry.
     */
    enum Permissions {
        READ = 1 << 0,
        WRITE = 1 << 1
    };

}

/**
 * @class Key
 * @brief Provides access control for ECS commands based on permissions.
 *
 * The `Key` class encapsulates the permissions for reading or writing to
 * components in the ECS registry. It restricts access based on the 
 * defined permissions (`READ` or `WRITE`). It is used by commands and
 * systems.
 */
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
