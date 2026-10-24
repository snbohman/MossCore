/**
 * @file    commands/quit.hpp
 * @brief   Quit: ask the App to stop after the current frame.
 *
 *     cmd::Quit::init(key).quit();
 *
 * The frame in progress finishes (every remaining system still ticks), then
 * App::run returns.
 */

#pragma once

#include <moss/commands/command.hpp>

namespace moss::commands {

class Quit : public Command<Quit> {
public:
    Quit() = default;

    void quit() const { quitFlag() = true; }

    [[nodiscard]] bool requested() const { return quitFlag(); }
};

}
