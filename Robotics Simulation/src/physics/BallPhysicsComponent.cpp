#include "physics/BallPhysicsComponent.h"
#include "core/GameObject.h"

void BallPhysicsComponent::OnAdd()
{
	const Transform& transform = GetOwner()->GetTransform();

	BodyDefinition bodyDef
	{
		.position = transform.position,
		.name = "Basketball",
		.type = BodyType::DYNAMIC,
		.density = 3.328694f,
		.friction = 0.75f,
		.restitution = 0.75f,
		.shapeType = ShapeType::CIRCLE,
		.shape = {.circle = { (transform.size.x + transform.size.y) / 4.0f } } // ( x + y / 2) / 2 to get the average, then divide again the radius
	};

	//float radius = (transform.size.x + transform.size.y) / 4.0f; 
	//bodyDef.SetShapeCircle(radius);

	bodyId = PhysicsEngine.CreateBody(bodyDef);
}

void BallPhysicsComponent::Update()
{
	const Vector2 position = PhysicsEngine.GetBodyPosition(bodyId);
	const Radian rotation = PhysicsEngine.GetBodyRotation(bodyId);
	logger.Log(std::format("[BallPhysicsComponent] Updating ball physics body id={} to position=({}, {}), rotation={}",
		bodyId,
		position.x,
		position.y,
		rotation.toDegree()), LogLevel::TRACE);
	GetOwner()->SetPosition(position);
	GetOwner()->SetRotation(rotation);
}

void BallPhysicsComponent::OnRemove()
{
	logger.Log(std::format("[BallPhysicsComponent] Destroying ball physics body with id={}", bodyId), LogLevel::INFO);
	PhysicsEngine.DestroyBody(bodyId);
}

std::string BallPhysicsComponent::ToString() const
{
	return "<BallPhysicsComponent>";
}
