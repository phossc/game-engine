# Entity Component System

## Introduction
Games in this engine are developed using components and entities. Components
make up the functionality and are combined in entities. For example, a player
might be an entity that contains a component providing movement functionality
and a component providing shooting functionality. If necessary, components
can depend on each other to ensure that some functionality is always present.
You might want your movement component to depend on a transform component so
that you can update the position of the player.

## Components

### Simple Component
```
#include <engine/core/component.hpp>

class Simple_component : public engine::core::Component {
public:
    COMPONENT("559f58a8-621e-433b-b9b5-07b3789a859b");
    DEPENDENCIES();

    void activate() override; // optional
    void deactivate() override; // optional
};
```

All components must derive directly or indirectly from `Component` which is
part of the engine core. All components must provide a `COMPONENT("<UUID>");`
macro specifying a unique identifier for the component. This UUID must match
the above format and can be generated with `uuidgen` or with Visual Studio.
Components must also provide a `DEPENDENCIES();` macro describing the
dependencies of the component. If the component has no dependencies, then the
list may remain empty like the above example - more on dependencies later.

### Dependent Component
```
class Dependent_component : public engine::core::Component {
public:
    COMPONENT("37c01f0e-2499-49c4-ba25-11fdc9812e01");
    DEPENDENCIES(Simple_component::s_uuid(), Another_component::s_uuid());

    void activate() override; // optional
    void deactivate() override; // optional
};
```

If a component is dependent on other components, then that component will
never exist on an entity without those components being present too.
Components can optionally do setup/teardown when entities are activated or
deactivated. This is done through `activate()` and `deactivate()`. The order
of `activate()` calls are based on the dependencies of an entity's
components. A component is always activated after its dependencies.
`deactivate()` is called in the reverse order of activation. Therefore, while
a component is active, its dependencies are also active.

### Component Registration
In order for components to be created, they must first be registered with the
component registry. The component registry maintains the component dependency
graph and helps ensure that components are never without their dependencies
in entities. Components can be registered anywhere but it is recommended to
register them in either of the following functions. 

```
Game::register_components() // For game components.
Editor::register_components() // For editor components.
Launcher::register_components() // For launcher components.
System::register_components() // For engine components.
```

Project specific components go in `Game::register_components()` where as the
other three registration points are used for extending the engine. However,
the engine can also be extended from the game registration point.

### Accessing Components
To get access to component dependencies and other components that might be
present on an entity, the following functions can be used.

```
// Through an entity
player_entity->get<Movement_component>();

// From within a component class
dependency<Transform_component>();

// Alternative to the above method
entity()->get<Transform_component>();
```

These methods return a pointer to the requested component if it is present
otherwise a null pointer is returned. It is not always necessary to check
whether the returned pointer is null. If a component depends on another
component and that other component is requested, then that pointer is
guaranteed to not be null.

The above methods all have non-template variants taking a UUID as an
argument. The return type of these methods is a `Component*` and not a
pointer to a specific component like the templated versions.

#### System Components
Engine subsystems are organized into components residing in the system
entity. These can be accessed in two ways.

```
using engine::core::sys;
using engine::Physics_system;

// Through the system entity
auto physics = sys->entity()->get<Physics_system>();

// Shorthand syntax
auto physics = sys->component<Physics_system>();
```

## Entities