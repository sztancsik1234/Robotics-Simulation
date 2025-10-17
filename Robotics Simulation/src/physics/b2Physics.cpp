#include "physics/b2Physics.h"

void b2Physics::Initialize()
{
    b2::World::Params worldParameters = b2::World::Params();
    worldParameters.gravity = { 0.f,  -9.8f };
    world = b2::World(worldParameters);
}

void b2Physics::Shutdown()
{
    world.Destroy();
}

void b2Physics::simulateStep(float deltaSeconds)
{
    world.Step(deltaSeconds, 4);
}

BodyId b2Physics::createBody(const BodyDefinition& bodyDef)
{
	auto bodyparams = bodydefToB2Params(bodyDef);
	auto bodyId = b2CreateBody(world.Handle(), &bodyparams);
    switch (bodyDef.shapeType)
    {
        case ShapeType::Circle: { CreateCircleShape(bodyId, bodyDef); break; }
        case ShapeType::Rectangle: 
            {
                // To be implemented later
                logger.Log("[b2Physics] Rectangle shape creation not yet implemented", LogLevel::ERROR);
                break; 
	        }
        default:
        {
                logger.Log("[b2Physics] Error: BodyDefinition has no valid shape type for body creation", LogLevel::ERROR);
                throw PhysicsObjectCreationFailedException("BodyDefinition has no valid shape type for body creation");
	    }
    }

    return BodyId();
}

void b2Physics::destroyBody(BodyId id)
{
    b2DestroyBody(static_cast<b2BodyId>(id));
}

Vector2 b2Physics::getBodyPosition(BodyId id) const
{
	b2Vec2 position = b2Body_GetPosition(static_cast<b2BodyId>(id));
    return Vector2(position.x, position.y);
}

Radian b2Physics::getBodyRotation(BodyId id) const
{
	b2Rot rotation = b2Body_GetRotation(static_cast<b2BodyId>(id));
	float angleInRadians = b2Rot_GetAngle(rotation);
    return angleInRadians;
}

void b2Physics::applyForceToBody(BodyId id, const Vector2& force, float timeWindow)
{}

void b2Physics::setSpeed(BodyId id, const Vector2 & impulse)
{}

b2::Body::Params b2Physics::bodydefToB2Params(BodyDefinition def) const
{
    b2::Body::Params params;
    params.allowFastRotation    = def.Wheel;
    params.angularDamping       = def.anglularDamping;
    params.fixedRotation        = def.isRotationFixed;
    params.isBullet             = def.isBullet;
    params.linearDamping        = def.linearDamping;
    params.linearVelocity       = def.initialVelocity;
    params.name                 = def.name;
    params.position             = def.position;
    params.rotation             = { .c = (float)def.rotation.cosine(),
                                    .s = (float)def.rotation.sine() };
    params.sleepThreshold       = def.sleepTreshold;
    params.type                 = nativeToB2Bodytype(def.type);

    return params;
}

b2::Shape::Params b2Physics::bodydefToB2ShapeParams(const BodyDefinition& bodyDef) const
{
    b2::Shape::Params shapeParams;
    shapeParams.density           = bodyDef.density;
    shapeParams.friction          = bodyDef.friction;
    shapeParams.restitution       = bodyDef.restitution;
    shapeParams.rollingResistance = bodyDef.rollingFriction;
    shapeParams.tangentSpeed      = bodyDef.tangentSpeed;
	return shapeParams;
}

b2BodyType b2Physics::nativeToB2Bodytype(BodyType inType) const
{
    switch (inType)
    {
    case (BodyType::Static):  return b2_staticBody; 
    case (BodyType::Dynamic): return b2_dynamicBody; 
    case (BodyType::Kinematic): return b2_kinematicBody; 
    }
	logger.Log("[b2Physics] Error: Invalid BodyType provided for conversion to b2BodyType", LogLevel::ERROR);
	throw std::runtime_error("Invalid BodyType provided for conversion to b2BodyType");
}

b2ShapeId b2Physics::CreateCircleShape(b2BodyId bodyId, const BodyDefinition& bodyDef) const
{
#ifdef _DEBUG
    if (bodyDef.shapeType != ShapeType::Circle)
    {
		logger.Log("[b2Physics] Error: Attempted to create circle shape with non-circle BodyDefinition", LogLevel::ERROR);
		throw PhysicsObjectCreationFailedException("Attempted to create a circle, but BodyDefinition shape type is not Circle");
    }
#endif

    b2Circle circle
    {
		.center = bodyDef.position, // If ancother position is desired, don't use this function
        .radius = bodyDef.shape.circle.radius
    };

	b2::Shape::Params shapeDef = bodydefToB2ShapeParams(bodyDef);
    auto returnId = b2CreateCircleShape(bodyId, &shapeDef, &circle);
	return returnId;
}
