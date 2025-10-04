#pragma once
#include "input/IInputService.h"
#include "core/GameObject.h"
#include "core/ComponentDTOs.h"

class MouseFollowerComponent final : public Component
{
public:
	MouseFollowerComponent(GameObject* owner, IInputService& inputService) :
		Component(owner), InputService(inputService)
	{}

	MouseFollowerComponent(GameObject* owner, IInputService& inputService,
		const MouseFollowerComponentDTO) :
		Component(owner), InputService(inputService)
	{}

	~MouseFollowerComponent() override = default;

	void OnAdd() override;
	void Update() override;
	void OnRemove() override;

private:
	IInputService& InputService;
};