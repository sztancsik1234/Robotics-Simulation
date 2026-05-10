#include "input/GhostComponent.h"
#include "input/MouseFollowerComponent.h"
#include "events/GhostCreatedEvent.h"
#include "events/GhostPlacedEvent.h"
#include <format>

GhostComponent::GhostComponent(GameObject* owner,
								const IViewport& viewport,
								IInputService& inputService,
								CentralMessageDispatcher& dispatcher)
	: Component(owner), viewport(viewport), inputService(inputService), dispatcher(dispatcher)
{}

void GhostComponent::OnAdd()
{
	auto* owner = GetOwner();

	// Strip and store every non-render component from the owner
	storedComponents = ExtractNonRenderComponents();

	// Add MouseFollowerComponent so the ghost tracks the mouse
	owner->EmplaceComponent<MouseFollowerComponent>(viewport, inputService);

	// Subscribe to click events
	inputService.mouseClickBroadcast.subscribe(this);

	// Broadcast GhostCreatedEvent
	GhostCreatedEvent createdEvt;
	dispatcher.ghostCreatedEventBroadcast.broadcast(&createdEvt);
}

void GhostComponent::OnRemove()
{
	inputService.mouseClickBroadcast.unsubscribe(this);
}

void GhostComponent::onNotify(ClickEvent* /*event*/)
{
	if (placed)
		return;

	placed = true;

	auto* owner = GetOwner();

	// Remove the MouseFollowerComponent
	owner->RemoveComponent<MouseFollowerComponent>();

	// Unsubscribe from click events before we potentially lose the component
	inputService.mouseClickBroadcast.unsubscribe(this);

	// Restore the original components
	RestoreComponents();

	// Broadcast GhostPlacedEvent
	GhostPlacedEvent placedEvt;
	dispatcher.ghostPlacedEventBroadcast.broadcast(&placedEvt);
}

void GhostComponent::ExtractNonRenderComponents()
{
	auto& ownerComponents = GetOwner()->
	return;
}

void GhostComponent::RestoreComponents()
{}

std::string GhostComponent::ToString() const
{
	return "<GhostComponent>";
}


