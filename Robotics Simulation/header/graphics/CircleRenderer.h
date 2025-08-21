#pragma once  
#include "RenderComponent.h"  
#include "core/GameObject.h"  

/// <summary>  
/// A mock class to render a circle on the screen.  
/// </summary>  
class CircleRenderer : public RenderComponent  
{  
public:  
    using RenderComponent::RenderComponent;  

    void Update() override;
};
