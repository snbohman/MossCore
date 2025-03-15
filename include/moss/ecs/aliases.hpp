/*

inline namespaces to abbreviate longer
namespaces.

Only available for code outside the ecs libary,
as it would imply self inclusion.

*/


#include <moss/ecs/components.hpp>
#include <moss/ecs/systems.hpp>
#include <moss/ecs/packages.hpp>
#include <moss/ecs/renderables.hpp>

namespace moss {

namespace cmp  { using namespace components; }
namespace rcmp { using namespace renderables; }
namespace pkg  { using namespace packages; }
namespace sys  { using namespace systems; }

}
