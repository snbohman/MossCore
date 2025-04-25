#pragma once

#include <moss/moss.hpp>
#include <components.hpp>


namespace tst {

namespace sys {

struct CompAttachA : moss::Component {
    u32 num;
};

}

class SysAttach : public moss::System {
public:
    void build(
        const moss::Key<moss::key::WRITE>& key,
        const moss::DynamicView& entities
    ) {
        moss::cmd::DynamicAttach<sys::CompAttachA>::init(key);
    }
};

}
