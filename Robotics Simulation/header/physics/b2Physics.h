#pragma once
#include "BodyDefinition.h"
#include "IPhysicsEngine.h"
#include "util/ILogger.h" 
#include <box2d/id.h>
#include <box2d/types.h>
#include "core/Angle.h"
#include "core/Vector2.h"
#include <list>
#include <vector>

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
	BodyId CreateBody(const BodyDefinition& body) override;
	void DestroyBody(BodyId id) override;
	Vector2 GetBodyPosition(BodyId id) const override;
	Radian GetBodyRotation(BodyId id) const override;
	void ApplyForceToBody(const BodyId id, const Vector2& force, float timeWindow) override;
	void SetSpeed(const BodyId id, const Vector2& impulse) override;

private:
	b2WorldId worldId = { 0 };
	b2BodyDef BodydefToB2BodyDef(const BodyDefinition& def) const;
	b2ShapeDef BodydefToB2ShapeDef(const BodyDefinition& bodyDef) const;
	[[nodiscard]] constexpr static b2BodyType NativeToB2Bodytype(const BodyType inType);
	b2ShapeId CreateCircleShape(b2BodyId bodyId, const BodyDefinition& bodyDef) const;
	b2ShapeId CreateBoxShape(b2BodyId bodyId, const BodyDefinition& bodyDef) const;
	[[nodiscard]] inline BodyId RegisterBodyId(b2BodyId b2Id);
	[[nodiscard]] inline const b2BodyId& GetB2BodyId(BodyId nativeId) const;
	void VerifyWorldInitialized() const;

	struct
	{
		std::vector<b2BodyId> bodies;
		std::list<BodyId> freeIds;

		BodyId _registerNewBody(b2BodyId id)
		{
			BodyId returnId;
			if (!freeIds.empty())
			{
				returnId = freeIds.front();
				freeIds.pop_front();
				bodies[returnId] = id;
			}
			else
			{
				returnId = static_cast<BodyId>(bodies.size());
				bodies.push_back(id);
			}
			return returnId;
		}

		void _freeBody(BodyId id)
		{
			bodies[id] = b2BodyId { 0 };
			freeIds.push_back(id);
		}

		const b2BodyId& operator[](BodyId id) const
		{
			return bodies.at(id);
		}
	} bodyrepository;

	ILogger& logger;

	bool isWorldInitialized = false;


};