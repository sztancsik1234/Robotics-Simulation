#pragma once

class World {
public:
	World();
	~World();
	void stepSimulation(float timeStep);
private:
	int Id;
};