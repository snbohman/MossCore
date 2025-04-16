ChatGPT generated. It's way to dramatic, but I don't care enough to write my own :)

# Moss Game Engine: Core Logic Overview

Moss is a game engine built with a focus on simplicity, flexibility, and efficiency. 
At its core, the engine is designed to manage game entities, components, and systems 
using the **Entity-Component-System (ECS)** architecture. This architecture is inspired 
by the likes of Bevy and Godot's scene systems, where entities represent objects in the 
game world, components hold data, and systems perform the logic.

In this overview, we will walk through the key components of the engine and how they 
interact to create a modular and extensible framework for game development.

---

## **1. Entity-Component-System (ECS)**

The engine follows the **ECS pattern** to manage game objects and logic. The ECS pattern 
divides the game objects into three core parts:

- **Entities**: These are unique identifiers representing game objects. They don't hold 
  data themselves, but they link to components.
  
- **Components**: These are the data containers associated with entities. A component 
  could store any type of data, such as position, velocity, or health.

- **Systems**: These are responsible for the logic that operates on entities. Systems 
  work by querying entities that have specific components and performing actions 
  based on that data.

### **How ECS Works in Moss:**
- **Entities** are created and managed by a `Mirror` class, which interacts with the ECS 
  registry.
- **Components** are attached to entities via the `Mirror` class, and they hold the 
  data needed by systems.
- **Systems** are connected to the context via the `Mirror`, and they are responsible 
  for updating entities based on the components attached to them.

---

## **2. Context and Mirror**

- **Context**: A context is a container that holds a set of systems and entities. It 
  represents a group of systems that work together to handle specific logic for a part 
  of the game. The `Context` class manages the lifecycle of the systems and entities, 
  such as initialization, ticking (updating), and cleanup.

- **Mirror**: The `Mirror` class acts as a bridge between the game engine's ECS registry 
  and the context. It is responsible for creating entities, attaching components, and 
  connecting systems. Through `Mirror`, entities and components are managed and can 
  be accessed by systems for processing.

### **Key Operations of the Mirror**:
- **create(count)**: Creates a specified number of entities.
- **attach(Components...)**: Attaches components to all entities in the `Mirror`’s view.
- **connect(Systems...)**: Connects systems to the context, allowing them to operate 
  on entities.

The `Mirror` class interacts with the `Context` class to group entities, components, and 
systems, ensuring that they can interact in a meaningful way.

---

## **3. Systems and Commands**

- **Systems** are the heart of the game logic. They perform actions based on the data 
  in the components. For example, a `PhysicsSystem` might update the position of 
  entities based on their velocity. A `HealthSystem` could check if any entities have 
  reached zero health and trigger events accordingly.

- **Commands**: To facilitate communication between systems and components, Moss 
  provides a system of **commands**. Commands allow systems to query entities and 
  components efficiently. These include:
    - **DynamicQuery**: Dynamically queries entities for specific components during 
      runtime.
    - **View**: Represents a view of entities that match certain component criteria.
    - **Query**: A static query that looks for entities with specific components.

The use of commands enables efficient and flexible interactions between systems and 
components, ensuring that they can access and modify the data they need without tightly 
coupling them together.

---

## **4. App Lifecycle**

The application in Moss is managed through the `App` class, which is responsible for the 
engine’s overall lifecycle. The `App` class orchestrates the initialization, building, 
running, and exiting phases.

- **init()**: Initializes the app and any necessary components.
- **build()**: Builds the app, setting up all contexts and systems.
- **run()**: Starts the main loop, running the app’s logic.
- **exit()**: Exits the app and cleans up all resources.

Within the `App` class, contexts are mounted and managed through the `mount()` method, 
ensuring that the right systems and entities are initialized at the right time.

---

## **5. Permissions System**

To add control over how systems interact with the components, the engine uses a 
**permissions system**. The `Key` class, defined by `Permissions` (read and write), 
ensures that systems and commands can only access the components they have permission 
to. This helps to prevent systems from making unintended changes to components and 
provides fine-grained control over the access rights of different systems.

- **Key<READ>**: Used when a system needs to query/view components.
- **Key<WRITE>**: Used when a system only needs to query components from a set view.

This system ensures that systems interact with the ECS registry in a secure and controlled 
manner. The build system for example only have a WRITE permission, as it is not ensured
that every component is initialized. The tick system has a READ key, as it is more or less 
continious and doesn't have the same risk of accessing old data.

---

## **6. Flexibility and Extensibility**

Moss is designed to be highly flexible and extensible. The fluent interface for managing 
contexts and systems allows for easy integration of new systems, components, and commands. 
The use of modern C++ features such as templates and type traits ensures that the engine 
is both performant and easy to extend with custom logic.

Additionally, the use of the **ECS** pattern makes it easy to add new features without tightly 
coupling systems or components together. If you need a new system or component, it can be 
added with minimal changes to existing code.

Unit testing is also easy. All context can be run, so if there is a problem with the player
logic, just run the player contex, and not all the others.

---

## **Conclusion**

Moss provides a robust framework for developing games with a focus on simplicity and 
flexibility. The use of ECS architecture ensures that the engine remains modular, scalable, 
and easy to maintain. By separating data (components), logic (systems), and objects 
(entities), Moss allows for powerful and efficient game development. With its permission 
system, fluent interface, and extensible design, Moss is ready to handle any game logic 
you need to implement.
