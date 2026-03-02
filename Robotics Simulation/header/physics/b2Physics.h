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
	b2BodyDef bodydefToB2BodyDef(const BodyDefinition& def) const;
	b2ShapeDef bodydefToB2ShapeDef(const BodyDefinition& bodyDef) const;
	[[nodiscard]] constexpr static b2BodyType nativeToB2Bodytype(const BodyType inType);
	b2ShapeId CreateCircleShape(b2BodyId bodyId, const BodyDefinition& bodyDef) const;
	b2ShapeId CreateBoxShape(b2BodyId bodyId, const BodyDefinition& bodyDef) const;
	[[nodiscard]] inline BodyId registerBodyId(b2BodyId b2Id);
	[[nodiscard]] inline const b2BodyId& getB2BodyId(BodyId nativeId) const;

	struct
	{
		std::vector<b2BodyId> bodies;
		std::list<BodyId> freeIds;

		BodyId _registerNewBody(b2BodyId id) {
			BodyId returnId;
			if (!freeIds.empty()) {
				returnId = freeIds.front();
				freeIds.pop_front();
				bodies[returnId] = id;
			}
			else {
				returnId = static_cast<BodyId>(bodies.size());
				bodies.push_back(id);
			}
			return returnId;
		}

		void _freeBody(BodyId id)
		{
			bodies[id] = b2BodyId{ 0 };
			freeIds.push_back(id);
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