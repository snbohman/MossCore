# Moss Game Engine: Core Architecture

Moss is a game engine built around the **Entity–Component–System (ECS)**
pattern. Its design emphasizes clarity, modularity, and testability. This
document introduces the core concepts: entities, components, systems, contexts,
and the supporting abstractions that make them work together.

---

## 1. Entity–Component–System (ECS)

Moss uses ECS to separate **data** (components) from **behavior** (systems).  

- **Entities** – Unique identifiers that represent objects in the game world.  
- **Components** – Plain data structures attached to entities (e.g. `Position`, `Velocity`, `Health`).  
- **Systems** – Functions that operate on entities with specific components. For example, a `PhysicsSystem` updates positions using velocities.  

### ECS in Moss
- Entities are created through a `Mirror`.  
- Components are attached to entities via the `Mirror`.  
- Systems access entities by declaring queries over components.  

---

## 2. Contexts and Mirrors

Moss groups entities and systems into **contexts**. A context represents a
specific area of logic (e.g. “player”, “physics”, or “UI”).  

- **Context** – Holds a collection of systems and entities. Handles
initialization, updates (ticks), and cleanup.  
- **Mirror** – A fluent interface for working with entities and components
inside a context. It creates entities, attaches components, and connects
systems.  

**Common operations on a `Mirror`:**
- `create(count)` – Creates entities.  
- `attach(Components...)` – Adds components to the entities in scope.  
- `connect(Systems...)` – Registers systems with the context.  

---

## 3. Systems and Queries

Systems contain the actual game logic. They read and modify components by declaring what data they require.  

Moss provides query abstractions to let systems express their needs clearly:  

- **Query** – A static query for entities with specific components (requires read access).  
- **DynamicQuery** – A runtime query, used when requirements are not known ahead of time.  
- **View** – A lightweight iterator over entities matching a set of components.  

These queries keep systems decoupled from the ECS internals while still being efficient.  

---

## 4. Application Lifecycle

The `App` class manages the overall engine lifecycle. It provides a consistent
entry point for initialization, building contexts, running the main loop, and
shutting down.  

- `init()` – Prepare global resources.  
- `build()` – Set up contexts, entities, and systems.  
- `run()` – Enter the main loop, ticking contexts in order.  
- `exit()` – Clean up resources.  

Contexts are mounted into the app via `mount()`. Each tick, the app calls into
the mounted contexts so their systems can process entities.  

---

## 5. Permissions System

Moss enforces controlled access to components using **permissions**. Each query
or system must declare whether it intends to **read** or **write** component
data.  

- **Key<READ>** – Grants read (and write) access. Used for ticking systems that rely on stable component data.  
- **Key<WRITE>** – Grants write-only access. Often used at build time when initializing entities.  

This ensures that systems cannot accidentally read uninitialized or stale data,
and it makes the engine safer in concurrent or incremental-update scenarios.  

---

## 6. Extensibility and Testing

Moss is designed for incremental extension. Adding new systems or components
requires minimal changes, since systems only depend on the components they
query.  

Contexts can be run independently, which makes it straightforward to write
**unit tests** for isolated parts of the game logic. For example, the player
context can be executed on its own without running the entire game.  

---

## Conclusion

Moss provides a small but solid ECS foundation:  
- Entities are lightweight IDs.  
- Components are plain data.  
- Systems express logic through queries.  
- Contexts and mirrors provide structure and ergonomics.  
- Permissions enforce safe access.  

This architecture keeps data and logic separated, encourages modular design, and makes testing straightforward.  
