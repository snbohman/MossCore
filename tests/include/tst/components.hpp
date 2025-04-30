#pragma once

#include <moss/moss.hpp>


namespace tst {

struct CompA : moss::Component {
    std::string uId;
    u32 num;

    CompA(const char* id, u32 n = 0) : uId(id), num(n) { }
};

struct CompB : moss::Component {
    std::string uId;
    u32 num;

    CompB(const char* id, u32 n = 0) : uId(id), num(n) { }
};

struct CompC : moss::Component {
    std::string uId;
    u32 num;

    CompC(const char* id, u32 n = 0) : uId(id), num(n) { }
};

struct CompD : moss::Component {
    u32 num;
};

}
