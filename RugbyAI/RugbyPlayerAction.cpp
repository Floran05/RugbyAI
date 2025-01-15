#include "RugbyPlayerAction.h"
#include "RugbyScene.h"
#include "Utils.h"
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

void RugbyPlayerAction_Support::OnStart(Player* pPlayer)
{
	TargetTryLanes = pPlayer->GetTag() == RugbyScene::Tag::PlayerGreen ? TRY_LANES_SCREEN_PERCENT : 1.f - TRY_LANES_SCREEN_PERCENT;
	ballCarrier = pPlayer->GetScene<RugbyScene>()->GetBall()->GetOwner();
}

void RugbyPlayerAction_Support::OnUpdate(Player* pPlayer)
{
	if (!ballCarrier || ballCarrier == pPlayer) return;
	float safeDistance = 50.0f;
	float followSpeed = PLAYER_SPEED;
	sf::Vector2f playerPos = pPlayer->GetPosition();
	sf::Vector2f carrierPos = ballCarrier->GetPosition();
	float targetX = TargetTryLanes;
	float targetY = playerPos.y;
	if (carrierPos.x < targetX && playerPos.x < carrierPos.x &&
		Utils::GetDistance(playerPos.x, playerPos.y, carrierPos.x, carrierPos.y) > safeDistance)
	{
		if (pPlayer->GoToPosition(targetX, targetY, followSpeed)) {
			return;
		}
	}
	else
	{
		followSpeed*=0.7;
	}
}

void RugbyPlayerAction_Support::OnEnd(Player* pPlayer)
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
