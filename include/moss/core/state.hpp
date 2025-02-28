#pragma once

#include <atomic>
#include <mutex>


namespace moss {

/// Singleton-pattern-esque design ///
class State {
public:
    enum GameState {
        PLAYING,
        MENU
    };

    struct StateData {
        GameState gameState;
        StateData() : gameState(MENU) { }
    };

public:
    static State& getInstance();
    StateData const& getState() const;

    template<typename Func>
    void updateState(Func&& func);

private:
    std::atomic<StateData*> currentState;
    mutable std::mutex mtx;
};

} // moss
