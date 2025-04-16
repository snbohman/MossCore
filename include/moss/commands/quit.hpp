/**
 * @file commands/quit.hpp
 * @brief Quit command
 */

#pragma once

#include <moss/core/key.hpp>
#include <moss/commands/primitives.hpp>


namespace moss::commands {

class Quit {
public:
    void apply(const Key<key::READ>& key) { m_quit = key.m_quit; } 
    void apply(const Key<key::WRITE>& key) { m_quit = key.m_quit; } 
    void clean() { m_quit = nullptr; }

    void quit(bool doClean = false) {
        M_ERROR_IFF(m_quit == nullptr,
            "Quit pointer is null. Not that apply must be called before any get method"
        );

        *m_quit = true;
        if (doClean) clean();
    }

private:
    bool* m_quit;
};

};
