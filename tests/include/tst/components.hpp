#pragma once

#include <moss/moss.hpp>


namespace tst {

struct CompA : moss::Component {
    const char* uId;

    CompA(const char* id) : uId(id) { }
};

struct CompB : moss::Component {
    std::string uId;

    CompB(const char* id) : uId(id) { }
};

struct CompC : moss::Component {
    std::string uId;

    CompC(const char* id) : uId(id) { }
};

}
