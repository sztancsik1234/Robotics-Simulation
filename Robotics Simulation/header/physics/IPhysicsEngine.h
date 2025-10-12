#pragma once
#include "physics/World.h"
#include "physics/BodyDefinition.h"
#include "box2d/box2cpp.h"

// Interface for a physics engine

using bodyId = unsigned int;

struct IPhysicsEngineBase
{ 
	virtual ~IPhysicsEngineBase() = default; 
};

struct SimulationLifecycle : IPhysicsEngineBase
{
	virtual void Initialize() = 0;
	virtual void Shutdown() = 0;
};

struct IWorldManager : SimulationLifecycle
{
	virtual void simulateStep(float deltaSeconds) = 0;
};

struct IBodyManager : IPhysicsEngineBase
{
	virtual bodyId createBody(const BodyDefinition& body) = 0;
	virtual void destroyBody(bodyId id) = 0;

	virtual Vector2 getBodyPosition(bodyId id) const = 0;
	virtual Radian getBodyRotation(bodyId id) const = 0;

	virtual void applyForceToBody(bodyId id, const Vector2& force, float timeWindow) = 0;
	virtual void setSpeed(bodyId id, const Vector2& impulse) = 0;
};

struct IJointManager : IPhysicsEngineBase
{
	// In the future, we can add methods to create and manage joints between bodies
};

struct IPhysicsEngine :
	SimulationLifecycle,
	IWorldManager,
	IBodyManager,
	IJointManager
{

};