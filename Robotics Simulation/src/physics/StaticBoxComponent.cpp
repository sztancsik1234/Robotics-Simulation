#include "physics/StaticBoxComponent.h"
#include "core/GameObject.h"

void StaticBoxComponent::OnAdd()
{
	const Transform& transform = GetOwner()->GetTransform();


#if 0
	BodyDefinition bodyDef;
	bodyDef.position = transform.position;		// BUG: This doesn't account for the anchor. The position of the body is currently the same as the position of the gameobject, which is the center of the box. If anchor is used, this needs to be changed to account for it.
	bodyDef.rotation = transform.rotation;
	bodyDef.type = BodyType::STATIC;
	bodyDef.SetShapeRectangle(transform.size);
#else
	BodyDefinition bodyDef
	{
		.position = transform.position,
		.rotation = transform.rotation,
		.type = BodyType::STATIC,
		.shapeType = ShapeType::RECTANGLE,
		.shape = {.rectangle = { transform.size.x, transform.size.y } }
	};
#endif
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
