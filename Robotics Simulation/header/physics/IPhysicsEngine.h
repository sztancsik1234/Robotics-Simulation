#pragma once
#include "physics/BodyDefinition.h"
#include "core/Angle.h"
#include "core/Vector2.h"
#include <stdexcept>

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
	virtual BodyId CreateBody(const BodyDefinition& body) = 0;
	virtual void DestroyBody(BodyId id) = 0;

	virtual Vector2 GetBodyPosition(BodyId id) const = 0;
	virtual Radian GetBodyRotation(BodyId id) const = 0;

	virtual void ApplyForceToBody(BodyId id, const Vector2& force, float timeWindow) = 0;
	virtual void SetSpeed(BodyId id, const Vector2& impulse) = 0;
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