#pragma once
#include "core/Component.h"
#include "core/GameObject.h"
#include "core/MessageDispatcher.h"
#include "graphics/IViewport.h"
#include "input/IInputService.h"
#include "util/IObserver.h"
#include "events/ClickEvent.h"
#include <list>
#include <memory>
#include <string>

/// <summary>
/// GhostComponent turns a GameObject into a "ghost":
///  - On add: strips non-render components, adds a MouseFollowerComponent,
///    and broadcasts a GhostCreatedEvent.
///  - On click: removes the MouseFollowerComponent, re-adds the stored
///    components, and broadcasts a GhostPlacedEvent.
/// </summary>
class GhostComponent final : public Component, public IObserver<ClickEvent>
{
public:
	GhostComponent(GameObject* owner,
				   ILogger& logger,
				   const IViewport& viewport,
				   IInputService& inputService,
				   CentralMessageDispatcher& dispatcher);

	~GhostComponent() override = default;

	void OnAdd() override;
	void Update() override {}
	void OnRemove() override;
	void Disable() override {}
	void Enable() override {}
	std::string ToString() const override;

	// IObserver<ClickEvent>
	void onNotify(ClickEvent* event) override;

private:
	ILogger& logger;
	const IViewport& viewport;
	IInputService& inputService;
	CentralMessageDispatcher& dispatcher;

	bool placed = false;
};
