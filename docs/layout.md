# Layout
A doc describing the different components/archtectures of the game engine, and
the core principles.


## [1] Core
### [1.2] App
The main application. The main hub. It's nothing fancy. An ecs registry, some
helper functions. It's all relativly simple. I have gone for a fancy sounding
*fluent Interface* pattern. Its all really preference, but I quite enjoy the
look and feel of it. **Non Functional** main pipeline example:

'''cpp
int main() {
    moss::App::init()
        .create() // creates entity e1
            .attachComponent<Comp1, Comp2>() // attaches to e1
        .create() // e2
            .attachSystem<Sys1, Sys2, Sys3>() // attaches to e2
        .create(2) // creates **two** entities (e3, e4)
            .attachComponent<Comp4>() // attaches to **e3 and e4**
}
'''

As you can see attaching components to specified entities is impossible.
attach[...] only attaches to the last created entity/entities. A static assert
is raised if a struct/class is inputed that doesnt inherit component/system
respectivly for their functions.

### [1.2] Contex
A singleton application used for systems interacting with the application. It
functions as a wrapper for the main apps registry, members, functions etc. It
is either provided as parameter of a system for ease of use, or aquired by the
system by a simple singleton get function. As the contex definition is quite
heavy, I wont be providing it, but feel free to look at '''core/contex.hpp'''
for the full definition.


## [2] Ecs
### [2.1] Component
A struct of data. Could be a position, could be a transform, could be a
material. Inherited from the component base class (an empty struct).

'''cpp
struct Transform : Component {
    vec3 position;
    vec3 scale;
    vec3 rotation;
}
'''

As structs and classes are virtually the same in cpp it doesnt really matter
what type you use, but I find it more clear to use a struct when describing pure
data and really signals a message to the user. Users can of course change this
to their preferences in their own created components, assuming they don't use
mossUI, but I could find a reason why.

### [2.2] System
Traditionally in ecs a system is a function. For my purposes, I have found that
it being a class brings more structure and in many cases less boilerplate. I
find it to be a good balance between structure and boilerplate. A simplified 
version of the system base class is defined as following.

'''cpp
class System {
public:
    virtual ~System() = default; 

    virtual void init() { }
    virtual void build() { }
    virtual void tick() { }
    virtual void exit() { }

    virtual void build(Contex& contex) { }
    virtual void tick(Contex& contex) { }
    virtual void exit(Contex& contex) { }
};
'''

The virtual destructor can be ignored, as it is common boilerplate for
inheritance classes. As you can see, four different functions are provided for
overriding, but only three with an overload of parameterer Contex. The contex
is, as described in 1.3 below, a singleton application/class, unanimous for
every part of the application. The contex can therefore also be aquired by a
simple get statement inside the system, but a parameter is provided for ease of
use.

A system can hold different 

### [2.3] Packages
A package is essentially a system, for calling systems and initializing
components. The base class very closely resembles the System class.
