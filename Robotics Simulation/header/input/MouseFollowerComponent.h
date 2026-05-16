#pragma once
#include "core/GameObject.h"
#include "graphics/IViewport.h"
#include "input/IInputService.h"
#include "core/Component.h"
#include <string>

class MouseFollowerComponent final : public Component
{
public:
	MouseFollowerComponent(GameObject* owner, const IViewport& camera, IInputService& inputService) :
		Component(owner), viewPort(camera), InputService(inputService)
	{}

	~MouseFollowerComponent() override = default;

	void OnAdd() override;
	void Update() override;
	void OnRemove() override;
	void Disable() override;
	void Enable() override;
	std::string ToString() const override;

	std::unique_ptr<Component> Clone(GameObject* newOwner) const override;

private:
	const IViewport& viewPort;
	IInputService& InputService;
	bool disabled = false;
};