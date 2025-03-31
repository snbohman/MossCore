#pragma once

#include <moss/core/contex.hpp>
#include <moss/core/fluent.hpp>


namespace moss {

class App {
public:
    void init();
    void build();
    void run();
    void clean();
    void exit();

    void apply(Contex<contex::READ>& contex);
    void apply(Fluent<fluent::WRITE>& fluent);

private:
    entt::registry m_registry;
    bool m_quit = false;
};

}
