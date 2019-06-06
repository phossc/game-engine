#ifndef ENGINE_CORE_UPDATE_SYSTEM_HPP
#define ENGINE_CORE_UPDATE_SYSTEM_HPP

#include <cstdint>
#include <map>
#include <unordered_map>

namespace engine::core {

struct Updatable {
    //! Delta time in seconds between frames.
    virtual void variable_update(double){};

    //! Progress towards newest simulation update.
    //!
    //! Called every frame with a value in the interval [0,1).
    virtual void interpolation_update(double){};

    //! Constant delta time in seconds between simulation updates.
    virtual void fixed_update(double){};
};

//! If an object wants to be updated it must first register with this system.
//! Registration/deregistration for a component might happen in the
//! activate/deactivate method of the component. Objects can only be registered
//! once for each update type.
class Update_system final {
public:
    void broadcast_variable_update(double dt);
    void broadcast_interpolation_update(double a);
    void broadcast_fixed_update(double dt);

    void register_variable_update(Updatable* updatable, std::int32_t priority);
    void deregister_variable_update(Updatable* updatable);

    void register_interpolation_update(Updatable* updatable,
                                       std::int32_t priority);
    void deregister_interpolation_update(Updatable* updatable);

    void register_fixed_update(Updatable* updatable, std::int32_t priority);
    void deregister_fixed_update(Updatable* updatable);

private:
    std::multimap<std::int32_t, Updatable*> variable_updatables_;
    std::multimap<std::int32_t, Updatable*> interpolation_updatables_;
    std::multimap<std::int32_t, Updatable*> fixed_updatables_;

    std::unordered_map<Updatable*, decltype(variable_updatables_)::iterator>
            iterators_to_variable_updatables_;

    std::unordered_map<Updatable*,
                       decltype(interpolation_updatables_)::iterator>
            iterators_to_interpolation_updatables_;

    std::unordered_map<Updatable*, decltype(fixed_updatables_)::iterator>
            iterators_to_fixed_updatables_;
};

} // namespace engine::core

#endif /* ENGINE_CORE_UPDATE_SYSTEM_HPP */