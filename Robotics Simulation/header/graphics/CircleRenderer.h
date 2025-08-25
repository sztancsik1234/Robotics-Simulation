#pragma once  
#include "RenderComponent.h"  
#include "core/GameObject.h"  

/// <summary>  
/// A mock class to render a circle on the screen.  
/// </summary>  
class CircleRenderer : public RenderComponent  
{  
public:  
    CircleRenderer(GameObject* owner, IPrimitiveRenderer& renderer, ILogger& logger) :
		RenderComponent(owner, logger),
        Renderer(renderer)
    {}

    void Update() override;

private:
	IPrimitiveRenderer& Renderer;
};
