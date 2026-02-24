#pragma once
#include "IPhysicsEngine.h"
#include "box2d/box2cpp.h"
#include "util/ILogger.h" 
#include <vector>
#include <queue>

class b2Physics : public IPhysicsEngine
{
public:
	explicit b2Physics(ILogger& logger) :
		logger(logger)
	{}

	// Inherited via IPhysicsEngine
	void Initialize() override;
	void Shutdown() override;
	bool IsInitialized() const override;
	void simulateStep(float deltaSeconds) override;
	BodyId createBody(const BodyDefinition& body) override;
	void destroyBody(BodyId id) override;
	Vector2 getBodyPosition(BodyId id) const override;
	Radian getBodyRotation(BodyId id) const override;
	void applyForceToBody(const BodyId id, const Vector2& force, float timeWindow) override;
	void setSpeed(const BodyId id, const Vector2& impulse) override;

private:
	b2WorldId worldId;
	b2::Body::Params bodydefToB2Params(BodyDefinition bodydef) const;
	b2::Shape::Params bodydefToB2ShapeParams(const BodyDefinition& bodyDef) const;
	[[nodiscard]] b2BodyType nativeToB2Bodytype(BodyType inType) const;
	b2ShapeId CreateCircleShape(b2BodyId bodyId, const BodyDefinition& bodyDef) const;
	b2ShapeId CreateBoxShape(b2BodyId bodyId, const BodyDefinition& bodyDef) const;
	[[nodiscard]] inline BodyId registerBodyId(b2BodyId b2Id);
	[[nodiscard]] inline const b2BodyId& getB2BodyId(BodyId nativeId) const;

	struct
	{
		std::vector<b2BodyId> bodies;
		std::queue<BodyId> freeIds;
		BodyId _registerNewBody(b2BodyId id) {
			BodyId idToReturn;
			if (!freeIds.empty()) {
				idToReturn = freeIds.front();
				freeIds.pop();
				bodies[idToReturn] = id;
			}
			else {
				idToReturn = static_cast<BodyId>(bodies.size());
				bodies.push_back(id);
			}
			return idToReturn;
		}
		void _freeBody(BodyId id) {
			bodies[id] = b2BodyId{ 0 };
			freeIds.push(id);
		}

		const b2BodyId& operator[](BodyId id) const {
			return bodies.at(id);
		}
	} bodyrepository;

	ILogger& logger;
#ifdef _DEBUG
	bool isWorldInitialized = false;
	void VerifyWorldInitialized() const;
#endif // _DEBUG

};