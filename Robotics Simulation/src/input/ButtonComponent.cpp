#include "input/ButtonComponent.h"
#include "input/ClickEvent.h"
#include "core/GameObject.h"
#include "core/Transform.h"
#include <format>

void ButtonComponent::onNotify(ClickEvent* e)
{
    Vector2 mousePos = e->pixelPosition;
    if (isInBounds(mousePos)) clicked = true;
}

void ButtonComponent::onNotify(MouseMoveEvent* event)
{
	lastPosition = event->Position;
}

bool ButtonComponent::isInBounds(Vector2& mousePos)
{
    const Transform& t = GetOwner()->GetTransform();
    const Vector2& anchor = GetOwner()->GetAnchor();

    // Axis-aligned bounding box check using the transform's position (top-left) and size.
    const Vector2 topLeft = t.position - Vector2(anchor.x * t.size.x, anchor.y * t.size.y);
    const Vector2 bottomRight = t.position + Vector2((1 - anchor.x) * t.size.x, (1 - anchor.y) * t.size.y);

    return (mousePos.x >= topLeft.x && mousePos.x <= bottomRight.x &&
        mousePos.y >= topLeft.y && mousePos.y <= bottomRight.y);
}

void ButtonComponent::OnAdd()
{
    inputService.mouseClickSubject.subscribe(this);
}

void ButtonComponent::Update()
{
    isHovered = isInBounds(lastPosition);
    if (clicked)
    {
        onClick();
        clicked = false;
    }
}

void ButtonComponent::OnRemove()
{
    inputService.mouseClickSubject.unsubscribe(this);
}

std::string ButtonComponent::ToString() const
{
    return "<ButtonComponent>";
}
