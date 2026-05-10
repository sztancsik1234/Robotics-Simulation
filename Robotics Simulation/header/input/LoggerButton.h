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
private:
    ILogger& logger;
};

