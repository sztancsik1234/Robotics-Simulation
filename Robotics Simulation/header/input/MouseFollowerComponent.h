#pragma once
#include "input/IInputService.h"
#include "core/GameObject.h"
#include "core/ComponentDTOs.h"
#include "graphics/Camera.h"

class MouseFollowerComponent final : public Component
{
public:
	MouseFollowerComponent(GameObject* owner, IViewport& camera, IInputService& inputService) :
		Component(owner), viewPort(camera), InputService(inputService)
	{}

	~MouseFollowerComponent() override = default;

	void OnAdd() override;
	void Update() override;
	void OnRemove() override;
	std::string ToString() const override;

private:
	IViewport& viewPort;
	IInputService& InputService;
};