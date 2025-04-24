/*
 * @fila    commands/command.hpp
 * @brief   implements the *command* and *dynamic command* class
 *
 * Should not be confused with commands.hpp, which imports
 * all the stuff.
 */

#pragma once

#include <moss/commands/primitives.hpp>

namespace moss {

class Command {
public:
    virtual ~Command() = default;

    
};

class DynamicCommand {
public:
    virtual ~DynamicCommand() = default;
};

}
