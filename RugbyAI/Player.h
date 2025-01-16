#pragma once

#include "Entity.h"
#include "Ball.h"

#define SPEED_BOOST_INCREASE_PERCENT 1.3f

class Box;
template<typename T>
class StateMachine;

class Player : public Entity
{

protected:

	virtual void OnInitialize() override;
	virtual void OnUpdate() override;
	virtual void OnCollision(Entity* collidedWith) override;
	virtual void OnDestroy() override;

protected:

	StateMachine<Player>* mpStateMachine;

	enum State
	{
		Start,
		Try,
		Support,
		Pass,
		Defense,

		Count
	};

	sf::String mName;

	Box* mDisplacementBoundingBox;

	float mInvincibilityDuration;
	float mSpeedBoostDuration;

	bool mIsInvincible;

public:

	void SetName(const sf::String& name) { mName = name; }
	sf::String GetName() const { return mName; }

	void SetDisplacementBoundingBox(Box* displacementBox) { mDisplacementBoundingBox = displacementBox; };
	Box* GetDisplacementBoundingBox() const { return mDisplacementBoundingBox; }

	bool HasBall() const;
	void PassBall(Player* target);

	int GetTag() const { return mTag; }
	float GetInvicibilityDuration() const { return mInvincibilityDuration; }
	float GetSpeedBoostDuration() const { return mSpeedBoostDuration; }

	void SetIsInvicible(bool isInvincible);
	bool IsInvincible() const { return mIsInvincible; }

	const char* GetStateName(State state);

};

