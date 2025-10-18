#include "physics/b2Physics.h"

void b2Physics::Initialize()
{
    b2WorldDef worldParameters = b2DefaultWorldDef();
    worldParameters.gravity = { 0.f,  -9.8f };
    worldId = b2CreateWorld(&worldParameters);
    logger.Log(std::format("[b2Physics] World:{} initialized.", worldId.index1), LogLevel::INFO);
}

void b2Physics::Shutdown()
{
    logger.Log(std::format("[b2Physics] World:{} is shutting down.", worldId.index1), LogLevel::INFO);
	b2DestroyWorld(worldId);
}

void b2Physics::simulateStep(float deltaSeconds)
{
    b2World_Step(worldId, deltaSeconds, 4);
}

BodyId b2Physics::createBody(const BodyDefinition& bodyDef)
{
	auto bodyparams = bodydefToB2Params(bodyDef);
	auto bodyId = b2CreateBody(worldId, &bodyparams);
    switch (bodyDef.shapeType)
    {
        case ShapeType::Circle:     { CreateCircleShape(bodyId, bodyDef); break; }
        case ShapeType::Rectangle:  { CreateBoxShape(bodyId, bodyDef);    break; }
        default:
        {
            logger.Log("[b2Physics] Error: BodyDefinition has no valid shape type for body creation",
                        LogLevel::ERROR);
            throw PhysicsObjectCreationFailedException("BodyDefinition has no valid shape type for body creation");
	    }
    }

    auto returnId = registerBodyId(bodyId);
    logger.Log(std::format("[b2Physics] Created body with id={} at position={}",
        returnId,
        std::string(bodyDef.position)), LogLevel::INFO);
    return returnId;
}

void b2Physics::destroyBody(BodyId id)
{
	const b2BodyId& b2id = getB2BodyId(id);
    b2DestroyBody(b2id);
	logger.Log(std::format("[b2Physics] Destroyed body with id={}", id), LogLevel::INFO);
}

Vector2 b2Physics::getBodyPosition(BodyId id) const
{
	b2Vec2 position = b2Body_GetPosition(getB2BodyId(id));
	const Vector2 vecPosition{ position.x, position.y };
	logger.Log(std::format("[b2Physics] Retrieved position={} for body id={}", std::string(vecPosition), id), LogLevel::TRACE);
    return vecPosition;
}

Radian b2Physics::getBodyRotation(BodyId id) const
{
	b2Rot rotation = b2Body_GetRotation(getB2BodyId(id));
	float angleInRadians = b2Rot_GetAngle(rotation);
	logger.Log(std::format("[b2Physics] Retrieved rotation={} radians for body id={}", angleInRadians, id), LogLevel::TRACE);
    return Radian(angleInRadians);
}

void b2Physics::applyForceToBody(const BodyId id, const Vector2& force, float timeWindow)
{
    logger.Log(std::format("[b2Physics] Applying force={} to body id={} over time window={}",
        std::string(force),
        id,
		timeWindow), LogLevel::TRACE);
    b2Body_ApplyForceToCenter(getB2BodyId(id), b2Vec2 { force.x, force.y }, true);
}

void b2Physics::setSpeed(const BodyId id, const Vector2 & impulse)
{
    logger.Log(std::format("[b2Physics] Setting speed={} for body id={}",
        std::string(impulse),
		id), LogLevel::TRACE);
    b2Body_SetLinearVelocity(getB2BodyId(id), b2Vec2 { impulse.x, impulse.y });
}

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
	params.rotation             = { .c = (float)def.rotation.cosine(),  // TODO: Rotation is mirrored in Box2D
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
    logger.Log(std::format("[b2Physics] Creating circle at location={}", std::string(bodyDef.position)), LogLevel::TRACE);
	return returnId;
}

b2ShapeId b2Physics::CreateBoxShape(b2BodyId bodyId, const BodyDefinition& bodyDef) const
{

#ifdef _DEBUG
    if (bodyDef.shapeType != ShapeType::Rectangle)
    {
        logger.Log("[b2Physics] Error: Attempted to create rectangle shape with non-rectangle BodyDefinition", LogLevel::ERROR);
        throw PhysicsObjectCreationFailedException("Attempted to create a rectangle, but BodyDefinition shape type is not Rectangle");
    }
#endif
    logger.Log(std::format("[b2Physics] Creating rectangle at location=({}, {}), size=({}, {})", bodyDef.position.x, bodyDef.position.y, bodyDef.shape.rectangle.width, bodyDef.shape.rectangle.height), LogLevel::INFO);

    const float halfWidth = 0.5f * bodyDef.shape.rectangle.width;
    const float halfHeight = 0.5f * bodyDef.shape.rectangle.height;

    b2Rot rot
    {
        .c = (float)bodyDef.rotation.cosine(),
        .s = (float)bodyDef.rotation.sine()
    };

    b2Polygon poly = b2MakeOffsetBox(halfWidth, halfHeight, bodyDef.position, rot);

    b2::Shape::Params shapeDef = bodydefToB2ShapeParams(bodyDef);
    auto returnId = b2CreatePolygonShape(bodyId, &shapeDef, &poly);
    return returnId;
}

BodyId b2Physics::registerBodyId(b2BodyId b2Id)
{
	return bodyrepository._registerNewBody(b2Id);
}

inline const b2BodyId& b2Physics::getB2BodyId(const BodyId nativeId) const
{
	return bodyrepository[nativeId];
}
