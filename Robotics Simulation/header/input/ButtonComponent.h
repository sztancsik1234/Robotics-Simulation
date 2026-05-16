#pragma once
#include "util/IObserver.h"
#include "core/Component.h"
#include "input/IInputService.h"
#include <functional>
#include <string>
#include "events/MouseMoveEvent.h"
#include "events/ClickEvent.h"

/// <summary>
/// An abstract component that observes mouse click events and invokes a callback when
/// the click falls within the owning GameObject's axis-aligned bounding box.
/// </summary>
class ButtonComponent : public Component, public IObserver<ClickEvent>, public IObserver<MouseMoveEvent>
{
public:

    ~ButtonComponent() override = default;

    // IObserver
    void onNotify(ClickEvent* event) override;
	void onNotify(MouseMoveEvent* event) override;


    // Component
    void OnAdd() final;
    void Update() final;
    void OnRemove() final;
    void Disable() override;
    void Enable() override;
    std::string ToString() const override;

protected:
    ButtonComponent(GameObject* owner, IInputService& inputService)
        : Component(owner), inputService(inputService)
    {}
    IInputService& GetInputService() const { return inputService; }
    bool isInBounds(Vector2& mousePos);
    virtual void onClick() = 0;
    virtual void onHovered() = 0;

	bool IsHovered() const { return isHovered; }


private:
    IInputService& inputService;

    Vector2 lastPosition;
	bool isHovered;
    bool clicked;
    void GrayOut();
    void Reenable();
};
