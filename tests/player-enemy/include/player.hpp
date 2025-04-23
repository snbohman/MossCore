#pragma once

#include <moss/moss.hpp>


class PlayerMovement : public moss::System {
public:
    void build(const moss::Key<moss::key::WRITE>& key, const moss::DynamicView& entities) override;
    void tick(const moss::Key<moss::key::READ>& key, const moss::DynamicView& entities) override;
};



class Player : public moss::Context {
public:
    void init(moss::Mirror& mirror) override;
};
