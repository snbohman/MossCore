#pragma once

#include <moss/moss.hpp>


namespace tst {

struct CompA : moss::Component {
    std::string str;
};

struct CompB : moss::Component {
    std::string str;
    u32 num;
};

struct CompC : moss::Component {
    std::string str;
    u32 num;
    f32 flt;
};

}
