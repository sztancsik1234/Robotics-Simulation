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
	GameObject* owner = GetOwner();

	try
	{
		logger.Log("[GhostComponent] Adding GhostComponent to gameObject: " + owner->ToString());
		owner->LogComponents();
		owner->GetComponent<BallPhysicsComponent>()->Disable();
	}
	catch (const ComponentNotFoundException&)
	{
		logger.Log("[GhostComponent] No BallPhysicsComponent found! Could not turn ghost", LogLevel::WARNING);
	}

	// Add MouseFollowerComponent so the ghost tracks the mouse
	logger.Log("Emplacing mousefollower");
	owner->EmplaceComponent<MouseFollowerComponent>(viewport, inputService);

	// Subscribe to click events
	inputService.mouseClickBroadcast.subscribe(this);

	// Broadcast GhostCreatedEvent
	GhostCreatedEvent createdEvt;
	dispatcher.ghostCreatedEventBroadcast.broadcast(&createdEvt);
}

void GhostComponent::Update()
{
	if (!placed) return;

	logger.Log("[GhostComponent] Triggered!");

	auto* owner = GetOwner();

	// Remove the MouseFollowerComponent
	owner->RemoveComponent(this);
}

void GhostComponent::OnRemove()
{
	inputService.mouseClickBroadcast.unsubscribe(this);

	try
	{
		GetOwner()->RemoveComponent<MouseFollowerComponent>();
	}
	catch (const ComponentNotFoundException&)
	{
		logger.Log("[GhostComponent] Could not remove MouseFollowerComponent while removing GhostComponent: GameObject has no MouseFollowerComponent.", LogLevel::ERROR);
	}

	try
	{
		GetOwner()->GetComponent<BallPhysicsComponent>()->Enable();
	}
	catch (const ComponentNotFoundException&)
	{
		logger.Log("[GhostComponent] could not re-enable BallPhysicsComponent. No ballPhysicsComponent was found!");
	}
}

void GhostComponent::Disable()
{
	inputService.mouseClickBroadcast.unsubscribe(this);
}

void GhostComponent::Enable()
{
	inputService.mouseClickBroadcast.subscribe(this);
}

void GhostComponent::onNotify(ClickEvent*)
{
	placed = true;
}

std::unique_ptr<Component> GhostComponent::Clone(GameObject* newOwner) const
{
	auto compPtr = std::make_unique<GhostComponent>(newOwner, logger, viewport, inputService, dispatcher);

	return compPtr;
}


std::string GhostComponent::ToString() const
{
	return "<GhostComponent>";
}


