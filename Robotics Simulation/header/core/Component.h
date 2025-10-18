#pragma once
#include <string>

class GameObject;

/// <summary>
/// Represents a base class for components that can be attached to a GameObject.
/// Provides lifecycle methods for initialization, updating, and removal.
/// </summary>
class Component
{
public:
	friend GameObject;

	/// <summary>
	/// Constructs a Component and associates it with a given GameObject.
	/// This method is called by the Game class.
	/// </summary>
	/// <param name="owner">Pointer to the GameObject that owns this Component.</param>
	explicit Component(GameObject* owner) : Owner(owner) {}

	virtual ~Component() = default;

	/// <summary>
	/// Called when the component is added to a GameObject. Called by the GameObject.
	/// This method should be overridden by derived classes to implement specific behavior.
	/// </summary>
	virtual void OnAdd() = 0;
	
	/// <summary>
	/// Called every game loop iteration to update the component's state.
	/// This method should be overridden by derived classes to implement specific behavior.
	/// </summary>
	virtual void Update() = 0;
	
	/// <summary>
	/// Called when an object is removed.
	/// Responsible for behaviour before cleaning up the component.
	/// Called by the GameObject's destuctor when it is destroyed or the component is removed.
	/// </summary>
	virtual void OnRemove() = 0;

	virtual std::string ToString() const = 0;

	virtual GameObject* GetOwner()		{ return Owner; }
	void SetOwner(GameObject* newOwner) { Owner = newOwner; }
private:
	GameObject* Owner;


};

