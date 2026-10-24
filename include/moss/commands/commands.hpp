/**
 * @file    commands/commands.hpp
 * @brief   Every command in one include.
 *
 *   Query     read components            Attach   add components
 *   Create    new entities               Detach   remove components
 *   Destroy   remove entities            Resource shared singletons
 *   Quit      stop the App
 */

#pragma once

#include <moss/commands/primitives.hpp>
#include <moss/commands/command.hpp>
#include <moss/commands/query.hpp>
#include <moss/commands/create.hpp>
#include <moss/commands/destroy.hpp>
#include <moss/commands/attach.hpp>
#include <moss/commands/detach.hpp>
#include <moss/commands/resource.hpp>
#include <moss/commands/quit.hpp>
