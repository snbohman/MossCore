/*
meta/defines.hpp

Defines log macros, utelizing spdlog.

*/


#pragma once

#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <entt/entt.hpp>

namespace moss {

#define M_ERROR(message, ...)   spdlog::error(message, __VA_ARGS__)
#define M_WARN(message, ...)    spdlog::warn(message, __VA_ARGS__)
#define M_INFO(message, ...)    spdlog::info(message, __VA_ARGS__)
#define M_DEB(message, ...)     spdlog::debug(message, __VA_ARGS__)

#define M_ERRORF(message)   spdlog::error(message)
#define M_WARNF(message)    spdlog::warn(message)
#define M_INFOF(message)    spdlog::info(message)
#define M_DEBF(message)     spdlog::debug(message)

#define M_ERROR_IF(statement, message, ...) if (statement) M_ERROR(message, __VA_ARGS__)
#define M_WARN_IF(statement, message, ...)  if (statement) M_WARN(message, __VA_ARGS__)
#define M_INFO_IF(statement, message, ...)  if (statement) M_ERROR(message, __VA_ARGS__)
#define M_DEB_IF(statement, message, ...)   if (statement) M_WARN(message, __VA_ARGS__)

#define M_ERROR_IFF(statement, message)  if (statement) M_ERRORF(message)
#define M_WARN_IFF(statement, message) if (statement) M_WARNF(message)
#define M_INFO_IFF(statement, message) if (statement) M_ERRORF(message)
#define M_DEB_IFF(statement, message)  if (statement) M_WARNF(message)

} // moss
