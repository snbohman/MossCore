/*
command/view.hpp

The view. Essentially just holding entities.
Applied in different ways, runtime or compile
time, but yet in the end just holds entities.

DynamicView:
The runtime view. No template needed. Is just a
heap allocated vector holding entities. No safety
implemented. Is defined in primitives for
preventing including problems and because it is
just a plain output type.

View:
The compile-time view. Of course, the registry is
runtime, but that is only shown in the apply
functions. Everything else is compile-time, which
automatically leads to some handy safety measures.


*/


#pragma once

#include <moss/core/contex.hpp>
#include <moss/commands/primitives.hpp>


namespace moss::commands::read {

template<typename... Inc, typename... Ex>
class View<Include<Inc...>, Exclude<Ex...>> {
    static_assert(sizeof...(Inc), "Include<> is required to have at least one component specified");

    void apply() { Contex<contex::READ>::get().apply(m_registry); } 
    void clean() { m_registry = nullptr; }

    [[nodiscard]] auto view(bool doClean = true) {
        static_assert(m_registry == nullptr, "Apply function must be called before aquiring view");

        if constexpr (sizeof...(Ex) == 0) {
            auto v = m_registry->view<Inc...>();
            if (doClean) clean();
            return std::move(v);
        } else {
            auto v = m_registry->view<Inc...>(entt::exclude<Ex...>);
            if (doClean) clean();
            return std::move(v);
        }
    }

private:
    entt::registry* m_registry = nullptr;
};

}
