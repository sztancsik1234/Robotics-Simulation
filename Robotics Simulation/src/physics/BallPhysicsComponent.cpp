#include "physics/BallPhysicsComponent.h"
#include "core/GameObject.h"

void BallPhysicsComponent::OnAdd()
{
	const Transform& transform = GetOwner()->GetTransform();

	BodyDefinition bodyDef
	{
		.position = transform.position,
		.name = "Basketball",
		.type = BodyType::Dynamic,
		.density = 3.328694f,
		.friction = 0.75f,
		.restitution = 0.75f,
	};

	float radius = (transform.size.x + transform.size.y) / 4.0f; // ( x + y / 2) / 2 to get the average, then divide again the radius
	bodyDef.SetShapeCircle(radius);

	bodyId = PhysicsEngine.createBody(bodyDef);
}

void BallPhysicsComponent::Update()
{
	const Vector2 position = PhysicsEngine.getBodyPosition(bodyId);
	const Radian rotation = PhysicsEngine.getBodyRotation(bodyId);
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
	PhysicsEngine.destroyBody(bodyId);
}

std::string BallPhysicsComponent::ToString() const
{
	return "<BallPhysicsComponent>";
}
