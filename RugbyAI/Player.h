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

	Ball* mBall;
	Box* mDisplacementBoundingBox;

	float mInvincibilityDuration;
	float mSpeedBoostDuration;

	bool mIsInvincible;

public:

	virtual void SetDisplacementBoundingBox(Box* displacementBox) { mDisplacementBoundingBox = displacementBox; };
	virtual Box* GetDisplacementBoundingBox() const { return mDisplacementBoundingBox; }

	virtual bool HasBall() const { return mBall != nullptr; }
	virtual void PassBall(Player* targetPlayer);
	virtual void RecoverBall(Ball* ball);

	int GetTag() const { return mTag; }
	float GetInvicibilityDuration() const { return mInvincibilityDuration; }
	float GetSpeedBoostDuration() const { return mSpeedBoostDuration; }

	void SetIsInvicible(bool isInvincible) { mIsInvincible = isInvincible; }
	bool IsInvincible() const { return mIsInvincible; }

	const char* GetStateName(State state);

};

