#include <moss/core/state.hpp>


moss::State& moss::State::getInstance() {
    static State instance;
    return instance;
}

moss::State::StateData const& moss::State::getState() const {
    return *currentState.load();
}

template<typename Func>
void moss::State::updateState(Func&& func) {
    std::lock_guard<std::mutex> lock(mtx);
    auto currentStatePtr = currentState.load();
    auto newState = std::make_unique<StateData>(*currentStatePtr);
    func(*newState);
    currentState.store(newState.release());
}
