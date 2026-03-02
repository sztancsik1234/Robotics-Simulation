#include "physics/b2Physics.h"

void b2Physics::Initialize()
{
    b2WorldDef worldParameters = b2DefaultWorldDef();
    worldParameters.gravity = { 0.f,  -9.8f };
    worldId = b2CreateWorld(&worldParameters);
    logger.Log(std::format("[b2Physics] World:{} initialized.", worldId.index1), LogLevel::INFO);
}

#ifdef _DEBUG
void b2Physics::VerifyWorldInitialized() const
{
	if (!isWorldInitialized)
	{
		logger.Log("[b2Physics] Attempted to use physics world before initialization.", LogLevel::ERROR);
		throw WorldNotInitializedException("Attempted to use physics world before initialization.");
	}
}
#endif // _DEBUG

void b2Physics::Shutdown()
{
    logger.Log(std::format("[b2Physics] World:{} is shutting down.", worldId.index1), LogLevel::INFO);
	b2DestroyWorld(worldId);
}

bool b2Physics::IsInitialized() const
{
	return isWorldInitialized;
}

void b2Physics::simulateStep(float deltaSeconds)
{
    b2World_Step(worldId, deltaSeconds, 4);
}

BodyId b2Physics::createBody(const BodyDefinition& bodyDef)
{
	auto bodyparams = bodydefToB2Params(bodyDef);
	auto bodyId = b2CreateBody(worldId, &bodyparams);
	//create body
	const b2BodyDef bodydef = bodydefToB2BodyDef(bodyDef);
	const b2BodyId bodyId = b2CreateBody(worldId, &bodydef);

	// create shape
    switch (bodyDef.shapeType)
    {
        case ShapeType::CIRCLE:     { CreateCircleShape(bodyId, bodyDef); break; }
        case ShapeType::RECTANGLE:  { CreateBoxShape(bodyId, bodyDef);    break; }
        default:
        {
            logger.Log("[b2Physics] Error: BodyDefinition has no valid shape type for body creation",
                        LogLevel::ERROR);
            throw PhysicsObjectCreationFailedException("BodyDefinition has no valid shape type for body creation");
	    }
    }

	// Create and track external ID
	BodyId returnId = registerBodyId(bodyId);
    logger.Log(std::format("[b2Physics] Created body with id={} at position={}",
        returnId,
        std::string(bodyDef.position)), LogLevel::INFO);

    return returnId;
}

void b2Physics::destroyBody(BodyId id)
{
	const b2BodyId& b2id = getB2BodyId(id);
	try
	{
		const b2BodyId b2id = getB2BodyId(id);
    b2DestroyBody(b2id);
		bodyrepository._freeBody(id);
	logger.Log(std::format("[b2Physics] Destroyed body with id={}", id), LogLevel::INFO);
}
	catch (const std::out_of_range& e)
	{
		logger.Log("Attempted to destroy a body that is not tracked: " + std::to_string(id) + " b2DestroyBody not called.", LogLevel::ERROR);
	}
}

Vector2 b2Physics::getBodyPosition(BodyId id) const
{
	b2Vec2 position = b2Body_GetPosition(getB2BodyId(id));
	b2BodyId b2id = getB2BodyId(id);
	b2Vec2 position = b2Body_GetPosition(b2id);
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

// TODO: timeWindow is currently ignored, as Box2D applies forces immediately. We can implement a more complex force application system in the future if needed.
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

// TODO: Make const return.
b2BodyDef b2Physics::bodydefToB2BodyDef(const BodyDefinition& inDef) const
{
	b2BodyDef outDef = b2DefaultBodyDef();
	outDef.allowFastRotation    = inDef.Wheel;
	outDef.angularDamping       = inDef.anglularDamping;
	outDef.fixedRotation        = inDef.isRotationFixed;
	outDef.isBullet             = inDef.isBullet;
	outDef.linearDamping        = inDef.linearDamping;
	outDef.linearVelocity       = inDef.initialVelocity;
	outDef.name                 = inDef.name;
	outDef.position             = inDef.position;
	outDef.rotation             = { .c = float(inDef.rotation.cosine()),  // TODO: Rotation is mirrored in Box2D
									.s = float(inDef.rotation.sine()) };
	outDef.sleepThreshold       = inDef.sleepTreshold;
	outDef.type                 = nativeToB2Bodytype(inDef.type);

	return outDef;
}

// TODO: Make const return.
b2ShapeDef b2Physics::bodydefToB2ShapeDef(const BodyDefinition& bodyDef) const
{
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density           = bodyDef.density;
	shapeDef.friction          = bodyDef.friction;
	shapeDef.restitution       = bodyDef.restitution;
	shapeDef.rollingResistance = bodyDef.rollingFriction;
	shapeDef.tangentSpeed      = bodyDef.tangentSpeed;
	return shapeDef;
}

constexpr b2BodyType b2Physics::nativeToB2Bodytype(const BodyType inType)
{
#if 0
    switch (inType)
    {
	case (BodyType::STATIC):    return b2_staticBody; 
	case (BodyType::DYNAMIC):   return b2_dynamicBody; 
    case (BodyType::KINEMATIC): return b2_kinematicBody; 
    }
	logger.Log("[b2Physics] Error: Invalid BodyType provided for conversion to b2BodyType", LogLevel::ERROR);
	throw std::runtime_error("Invalid BodyType provided for conversion to b2BodyType");
#else
	#ifdef _DEBUG
		if ((int)inType >= b2_bodyTypeCount) throw 	std::runtime_error("Invalid BodyType provided for conversion to b2BodyType");
	#endif // _DEBUG
	// type-punt the enum into b2BodyType
	return static_cast<b2BodyType>(inType);
#endif
}

b2ShapeId b2Physics::CreateCircleShape(b2BodyId bodyId, const BodyDefinition& bodyDef) const
{

#ifdef _DEBUG
    if (bodyDef.shapeType != ShapeType::CIRCLE)
    {
		logger.Log("[b2Physics] Error: Attempted to create circle shape with non-circle BodyDefinition", LogLevel::ERROR);
		throw PhysicsObjectCreationFailedException("Attempted to create a circle, but BodyDefinition shape type is not Circle");
    }
#endif

	const b2Circle circle
    {
		.center = bodyDef.position, // If another position is desired--for example at non-centered anchors--don't use this function
        .radius = bodyDef.shape.circle.radius
    };

	b2ShapeDef shapeDef = bodydefToB2ShapeDef(bodyDef);
    auto returnId = b2CreateCircleShape(bodyId, &shapeDef, &circle);
	logger.Log(std::format("[b2Physics] Creating circle at location={}", std::string(bodyDef.position)), LogLevel::INFO);
	return returnId;
}

b2ShapeId b2Physics::CreateBoxShape(b2BodyId bodyId, const BodyDefinition& bodyDef) const
{

#ifdef _DEBUG
    if (bodyDef.shapeType != ShapeType::RECTANGLE)
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

	// TODO: Check up how anchor works in box2d
	b2Polygon poly = b2MakeOffsetBox(halfWidth, halfHeight, bodyDef.anchor, rot);

	const b2ShapeDef shapeDef = bodydefToB2ShapeDef(bodyDef);
    auto returnId = b2CreatePolygonShape(bodyId, &shapeDef, &poly);
    return returnId;
}

// Register the b2BodyId and return the corresponding BodyId for external use.
BodyId b2Physics::registerBodyId(b2BodyId b2Id)
{
	return bodyrepository._registerNewBody(b2Id);
}

inline const b2BodyId& b2Physics::getB2BodyId(const BodyId nativeId) const
{
	return bodyrepository[nativeId];
}
