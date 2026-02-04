#pragma once
#include "input/IInputService.h"
#include "core/GameObject.h"
#include "core/ComponentDTOs.h"
#include "graphics/IViewport.h"

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
	std::string ToString() const override;

private:
	const IViewport& viewPort;
	IInputService& InputService;
};