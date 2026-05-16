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
	enabled = true;
}

void BallPhysicsComponent::Update()
{
	if (isDisabled()) return;

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
	enabled = false;
}

void BallPhysicsComponent::Disable()
{
	if (isDisabled()) return;

	PhysicsEngine.DestroyBody(bodyId);
	bodyId = 0;
	enabled = false;
}

void BallPhysicsComponent::Enable()
{
	if (isEnabled()) return;

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
	enabled = true;
}

std::string BallPhysicsComponent::ToString() const
{
	return "<BallPhysicsComponent>";
}

std::unique_ptr<Component> BallPhysicsComponent::Clone(GameObject* newOwner) const
{
	return std::make_unique<BallPhysicsComponent>(newOwner, logger, PhysicsEngine);
}
