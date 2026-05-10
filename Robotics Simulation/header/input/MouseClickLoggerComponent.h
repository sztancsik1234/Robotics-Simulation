#pragma once
#include "util/IObserver.h"
#include "util/ILogger.h"
#include "core/Component.h"
#include "input/IInputService.h"
class MouseClickLoggerComponent final :
    public Component, public IObserver<ClickEvent>
{
public:
	MouseClickLoggerComponent(GameObject* owner, ILogger& logger, IInputService& inputService)
		: Component(owner), logger(logger), inputService(inputService)
	{
	}
    void onNotify(ClickEvent* event) override;
	void OnAdd() override;
	void Update() override {}
	void OnRemove() override;
	std::string ToString() const override;

private:
    ILogger& logger;
	IInputService& inputService;
};

