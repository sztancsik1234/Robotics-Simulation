#include "physics/StaticBoxComponent.h"
#include "core/GameObject.h"

void StaticBoxComponent::OnAdd()
{
	const Transform& transform = GetOwner()->GetTransform();

	BodyDefinition bodyDef;
	bodyDef.position = transform.position;
	bodyDef.rotation = transform.rotation;
	bodyDef.SetShapeRectangle(transform.size);
	BodyId = PhysicsEngine.createBody(bodyDef);
	logger.Log(std::format("[StaticBoxComponent] Created static box body with id={} at position=({}, {}), size=({}, {})",
		BodyId,
		transform.position.x,
		transform.position.y,
		transform.size.x,
		transform.size.y), LogLevel::INFO);
}

void StaticBoxComponent::Update()
{
	// Static body, no need to update
}

void StaticBoxComponent::OnRemove()
{
	logger.Log(std::format("[StaticBoxComponent] Destroying static box body with id={}", BodyId), LogLevel::INFO);
	PhysicsEngine.destroyBody(BodyId);
}

std::string StaticBoxComponent::ToString() const
{
	return "<StaticBoxComponent>";
}
