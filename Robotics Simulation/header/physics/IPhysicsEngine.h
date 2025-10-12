#pragma once
#include "physics/World.h"

// Interface for a physics engine

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

struct IPrimitivePhysics
{

};

struct IPhysicsEngine :
	SimulationLifecycle,
	IWorldManager,
	IPrimitivePhysics
{

};