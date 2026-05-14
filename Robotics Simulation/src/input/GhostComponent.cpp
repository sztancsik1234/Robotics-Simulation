#include "input/GhostComponent.h"
#include "input/MouseFollowerComponent.h"
#include "events/GhostCreatedEvent.h"
#include "events/GhostPlacedEvent.h"
#include <format>
#include <physics/BallPhysicsComponent.h>

GhostComponent::GhostComponent(GameObject* owner,
								ILogger& logger,
								const IViewport& viewport,
								IInputService& inputService,
								CentralMessageDispatcher& dispatcher)
	: Component(owner), logger(logger), viewport(viewport), inputService(inputService), dispatcher(dispatcher)
{}

void GhostComponent::OnAdd()
{
	auto* owner = GetOwner();

	try
	{
		// Restore the original components
		owner->GetComponent<BallPhysicsComponent>()->Disable();
	}
	catch (const ComponentNotFoundException&)
	{
		logger.Log("No BallPhysicsComponent found! Could not turn ghost", LogLevel::WARNING);
	}

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

void GhostComponent::onNotify(ClickEvent*)
{
	if (placed)
		return;

	placed = true;

	auto* owner = GetOwner();

	// Remove the MouseFollowerComponent
	owner->RemoveComponent<MouseFollowerComponent>();

	// Unsubscribe from click events before we potentially lose the component
	inputService.mouseClickBroadcast.unsubscribe(this);

	try
	{
		// Restore the original components
		owner->GetComponent<BallPhysicsComponent>()->Enable();
	}
	catch (const ComponentNotFoundException&)
	{
		logger.Log("No BallPhysicsComponent found! Physics could not be re-enabled", LogLevel::WARNING);
	}


	// Broadcast GhostPlacedEvent
	GhostPlacedEvent placedEvt;
	dispatcher.ghostPlacedEventBroadcast.broadcast(&placedEvt);
}


std::string GhostComponent::ToString() const
{
	return "<GhostComponent>";
}


