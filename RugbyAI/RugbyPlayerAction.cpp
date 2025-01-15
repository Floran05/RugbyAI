#include "RugbyPlayerAction.h"
#include "RugbyScene.h"

void RugbyPlayerAction_Try::OnStart(Player* pPlayer)
{
	mInvincibilityTimer = pPlayer->GetInvicibilityDuration();
	mSpeedBoostTimer = pPlayer->GetSpeedBoostDuration();

	pPlayer->SetIsInvicible(true);
	pPlayer->SetSpeed(PLAYER_SPEED * SPEED_BOOST_INCREASE_PERCENT);
	pPlayer->SetDirection(1.f, 0.f);
}

void RugbyPlayerAction_Try::OnUpdate(Player* pPlayer)
{
	if (mInvincibilityTimer < 0.f && mSpeedBoostTimer < 0.f) return;
	const float deltaTime = GameManager::Get()->GetDeltaTime();
	mInvincibilityTimer -= deltaTime;
	mSpeedBoostTimer -= deltaTime;

	if (mInvincibilityTimer < 0.f)
	{
		pPlayer->SetIsInvicible(false);
	}
	if (mSpeedBoostTimer < 0.f)
	{
		pPlayer->SetSpeed(PLAYER_SPEED);
	}
}

void RugbyPlayerAction_Try::OnEnd(Player* pPlayer)
{
}


void RubgyPlayerAction_Pass::OnStart(Player* pPlayer)
{
	RugbyScene* scene = pPlayer->GetScene<RugbyScene>();
	if (Player* target = scene->GetBestTeammateForPass())
	{
		pPlayer->PassBall(target);
	}
}

void RubgyPlayerAction_Pass::OnUpdate(Player* pPlayer)
{
}

void RubgyPlayerAction_Pass::OnEnd(Player* pPlayer)
{
}
