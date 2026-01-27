#pragma once
#include <unordered_map>
#include "physics/IPhysicsEngine.h"
#include "physics/BodyDefinition.h"
#include "core/Vector2.h"
#include "core/Angle.h"

class MockPhysicsEngine final : public IPhysicsEngine
{
public:
    // IWorldManager
    void Initialize() override;
    void Shutdown() override;
    void simulateStep(float deltaSeconds) override;

    // IBodyManager
    BodyId createBody(const BodyDefinition& body) override;
    void destroyBody(BodyId id) override;

    Vector2 getBodyPosition(BodyId id) const override;
    Radian getBodyRotation(BodyId id) const override;

    void applyForceToBody(BodyId id, const Vector2& force, float timeWindow) override;
    void setSpeed(BodyId id, const Vector2& impulse) override;

    // Test inspection flags/state
    bool initializeCalled{ false };
    bool shutdownCalled{ false };
    bool simulateStepCalled{ false };
    float lastDeltaSeconds{ 0.f };

    bool createBodyCalled{ false };
    bool destroyBodyCalled{ false };
    bool applyForceCalled{ false };
    bool setSpeedCalled{ false };

    BodyId lastBodyId{ 0 };
    Vector2 lastAppliedForce{ 0.f, 0.f };
    float lastTimeWindow{ 0.f };
    Vector2 lastSetSpeed{ 0.f, 0.f };

private:
    struct BodyData
    {
        Vector2 position{ 0.f, 0.f };
        Radian rotation{ 0.f };
        Vector2 velocity{ 0.f, 0.f };
        BodyType type{ BodyType::STATIC };
    };

    std::unordered_map<BodyId, BodyData> bodies_;
    BodyId nextId_{ 1 };
    bool isInitialized_{ false };
};