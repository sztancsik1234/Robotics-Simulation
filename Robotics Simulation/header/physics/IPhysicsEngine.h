#pragma once
#include "physics/World.h"
#include "physics/BodyDefinition.h"
#include "box2d/box2cpp.h"

// Interface for a physics engine

using BodyId = unsigned int;

struct IPhysicsEngineBase
{ 
	virtual ~IPhysicsEngineBase() = default; 
};

struct IWorldManager : IPhysicsEngineBase
{
	virtual void Initialize() = 0;
	virtual void Shutdown() = 0;
	virtual void simulateStep(float deltaSeconds) = 0;
	virtual bool IsInitialized() const = 0;
};

struct IBodyManager : IPhysicsEngineBase
{
	virtual BodyId createBody(const BodyDefinition& body) = 0;
	virtual void destroyBody(BodyId id) = 0;

	virtual Vector2 getBodyPosition(BodyId id) const = 0;
	virtual Radian getBodyRotation(BodyId id) const = 0;

	virtual void applyForceToBody(BodyId id, const Vector2& force, float timeWindow) = 0;
	virtual void setSpeed(BodyId id, const Vector2& impulse) = 0;
};

struct IJointManager : IPhysicsEngineBase
{
	// In the future, we can add methods to create and manage joints between bodies
};

struct IPhysicsEngine :
	IWorldManager,
	IBodyManager,
	IJointManager
{

};

class PhysicsException : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

class PhysicsObjectCreationFailedException : public PhysicsException
{
public:
	using PhysicsException::PhysicsException;
};

class InvalidBodyIdException : public PhysicsException
{
public:
	using PhysicsException::PhysicsException;
};

class WorldNotInitializedException : public PhysicsException
{
public:
	using PhysicsException::PhysicsException;
};

class WorldAlreadyInitializedException : public PhysicsException
{
public:
	using PhysicsException::PhysicsException;
};