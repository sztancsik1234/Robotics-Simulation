#pragma once
#include "input/IInputService.h"
#include "core/GameObject.h"
#include "core/ComponentDTOs.h"
#include "graphics/Camera.h"

class MouseFollowerComponent final : public Component
{
public:
	MouseFollowerComponent(GameObject* owner, Camera& camera, IInputService& inputService) :
		Component(owner), camera(camera), InputService(inputService)
	{}

	~MouseFollowerComponent() override = default;

	void OnAdd() override;
	void Update() override;
	void OnRemove() override;
	std::string ToString() const override;

private:
	Camera& camera;
	IInputService& InputService;
};