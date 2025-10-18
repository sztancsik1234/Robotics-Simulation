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

	BodyId = PhysicsEngine.createBody(bodyDef);
}

void BallPhysicsComponent::Update()
{
	// set owner transform to match physics body
	GetOwner()->SetPosition(PhysicsEngine.getBodyPosition(BodyId));
	GetOwner()->SetRotation(PhysicsEngine.getBodyRotation(BodyId));
}

void BallPhysicsComponent::OnRemove()
{
	PhysicsEngine.destroyBody(BodyId);
}

std::string BallPhysicsComponent::ToString() const
{
	return "<BallPhysicsComponent>";
}
