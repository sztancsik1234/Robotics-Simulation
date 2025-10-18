#include "physics/StaticBoxComponent.h"
#include "core/GameObject.h"

void StaticBoxComponent::OnAdd()
{
	const Transform& transform = GetOwner()->GetTransform();

	BodyDefinition bodyDef;
	bodyDef.position = transform.position;
	bodyDef.rotation = transform.rotation;
	bodyDef.shape = std::make_unique<Rectangle>(transform.size);
	BodyId = PhysicsEngine.createBody(bodyDef);
}

void StaticBoxComponent::Update()
{
	// Static body, no need to update
}

void StaticBoxComponent::OnRemove()
{
	PhysicsEngine.destroyBody(BodyId);
}

std::string StaticBoxComponent::ToString() const
{
	return "<StaticBoxComponent>";
}
