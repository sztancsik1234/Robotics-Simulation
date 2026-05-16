#pragma once
#include "ButtonComponent.h"
#include <util/ILogger.h>
class LoggerButton final :
    public ButtonComponent
{
    public:
    LoggerButton(GameObject* owner, IInputService& inputService, ILogger& logger)
        : ButtonComponent(owner, inputService), logger(logger)
	{}
protected:
    void onClick() override;
    void onHovered() override;
    std::unique_ptr<Component> Clone(GameObject* newOwner) const override;
private:
    ILogger& logger;
};

