#ifndef ENGINE_SUBSYSTEMS_PHYSICS_SYSTEM_HPP
#define ENGINE_SUBSYSTEMS_PHYSICS_SYSTEM_HPP

#include "engine/core/component.hpp"
#include "engine/core/update_system.hpp"

#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "btBulletDynamicsCommon.h"

namespace engine {

class Physics_system : public core::Component, public core::Updatable {
public:
    COMPONENT("bf9aa9be-2fa9-4098-b4b8-25bfe2057ac4");
    DEPENDENCIES();

    Physics_system();

    void activate() override;
    void deactivate() override;

    void fixed_update(double dt) override;
    void interpolation_update(double a) override;

    btSoftRigidDynamicsWorld& world() noexcept { return world_; }

private:
    // This subclass resets m_localTime after each simulation step making it
    // easier to integrate bullet with the game loop of the engine.
    class softRigidDynamicsWorld : public btSoftRigidDynamicsWorld {
    public:
        using btSoftRigidDynamicsWorld::btSoftRigidDynamicsWorld;

        int stepSimulation(btScalar timeStep, int maxSubSteps,
                           btScalar fixedTimeStep) override {
            int num_steps = btDiscreteDynamicsWorld::stepSimulation(
                    timeStep, maxSubSteps, fixedTimeStep);
            m_localTime = 0.0;
            return num_steps;
        }
    };

    btDefaultCollisionConfiguration collisionConfig_;
    btCollisionDispatcher dispatcher_;
    btDbvtBroadphase broadphase_;
    btSequentialImpulseConstraintSolver solver_;
    softRigidDynamicsWorld world_;
};

} // namespace engine

#endif /* ENGINE_SUBSYSTEMS_PHYSICS_SYSTEM_HPP */