# Entity Component System

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
