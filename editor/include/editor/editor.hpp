#ifndef EDITOR_EDITOR_HPP
#define EDITOR_EDITOR_HPP

#include "engine/core/component.hpp"

namespace editor {

class Editor final : public engine::core::Component {
public:
    COMPONENT("d49d0588-5ed5-46d2-9525-48493a024d93");
    DEPENDENCIES();

    //! Functions as the editor's entry point.
    void activate() override;
    void deactivate() override;
};

} // namespace editor

#endif /* EDITOR_EDITOR_HPP */
