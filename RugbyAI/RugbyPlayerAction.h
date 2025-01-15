#pragma once

#include "Action.h"
#include "Player.h"

class RugbyPlayerAction_Try : public Action<Player>
{
public:
	void OnStart(Player* pPlayer) override;
	void OnUpdate(Player* pPlayer) override;
	void OnEnd(Player* pPlayer) override;

protected:

	float mInvincibilityTimer;
	float mSpeedBoostTimer;

};
class RugbyPlayerAction_Support : public Action<Player>
{
public:
	void OnStart(Player* pPlayer) override;
	void OnUpdate(Player* pPlayer) override;
	void OnEnd(Player* pPlayer) override;
protected :
	float mTargetTryLanes;
	Player* mballCarrier;
	float mfollowSpeed;
	float mTargetX;
	float mTargetY;
};

class RubgyPlayerAction_Pass : public Action<Player>
{
public:
	void OnStart(Player* pPlayer) override;
	void OnUpdate(Player* pPlayer) override;
	void OnEnd(Player* pPlayer) override;
};
	
