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

    void load(Contex<contex::READ>& contex);
    void load(Contex<contex::WRITE>& contex);
    void load(Fluent<fluent::READ>& contex);
    void load(Fluent<fluent::WRITE>& contex);

private:
    entt::registry m_registry;
};

}
