#pragma once
#include "IPhysicsEngine.h"
#include "box2d/box2cpp.h"
#include "util/ILogger.h" 

class b2Physics : public IPhysicsEngine
{
public:
	b2Physics(ILogger& logger) :
		logger(logger)
	{}

	// Inherited via IPhysicsEngine
	void Initialize() override;
	void Shutdown() override;
	void simulateStep(float deltaSeconds) override;
	BodyId createBody(const BodyDefinition& body) override;
	void destroyBody(BodyId id) override;
	Vector2 getBodyPosition(BodyId id) const override;
	Radian getBodyRotation(BodyId id) const override;
	void applyForceToBody(BodyId id, const Vector2& force, float timeWindow) override;
	void setSpeed(BodyId id, const Vector2& impulse) override;

private:
	b2::World world;
	b2::Body::Params bodydefToB2Params(BodyDefinition bodydef) const;
	b2::Shape::Params bodydefToB2ShapeParams(const BodyDefinition& bodyDef) const;
	b2BodyType nativeToB2Bodytype(BodyType inType) const;
	b2ShapeId CreateCircleShape(b2BodyId bodyId, const BodyDefinition& bodyDef) const;
	b2ShapeId CreateBoxShape(b2BodyId bodyId, const BodyDefinition& bodyDef) const;

	ILogger& logger;
};