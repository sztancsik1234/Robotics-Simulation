#include "physics/StaticBoxComponent.h"
#include "core/GameObject.h"

void StaticBoxComponent::OnAdd()
{
	Transform* transform = GetOwner()->GetTransform();
	if (transform == nullptr)
	{
		throw std::runtime_error("StaticBoxComponent requires a Transform component.");
	}
	BodyDefinition bodyDef;
	bodyDef.position = transform->position;
	bodyDef.rotation = transform->rotation;
	bodyDef.shape = std::make_unique<Rectangle>(transform->size);
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
