/**
 * @file    moss.hpp
 * @brief   Umbrella header: everything an application or extension needs.
 *
 *   App, Context, Mirror         lifecycle and setup
 *   System, Key                  logic and its permission token
 *   cmd::Query / Attach / ...    the commands (see commands/commands.hpp)
 *   M_ERROR / M_FATAL / ...      logging and the fatal-error policy
 */

#pragma once

#include <moss/meta/libs.hpp>
#include <moss/meta/types.hpp>
#include <moss/meta/logs.hpp>
#include <moss/core/key.hpp>
#include <moss/commands/commands.hpp>
#include <moss/ecs/ecs.hpp>
#include <moss/core/context.hpp>
#include <moss/core/mirror.hpp>
#include <moss/core/app.hpp>
#include <moss/meta/aliases.hpp>
