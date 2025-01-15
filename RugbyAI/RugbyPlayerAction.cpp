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
	mTargetTryLanes = pPlayer->GetTag() == RugbyScene::Tag::PlayerGreen ? TRY_LANES_SCREEN_PERCENT : 1.f - TRY_LANES_SCREEN_PERCENT;
	mballCarrier = pPlayer->GetScene<RugbyScene>()->GetBall()->GetOwner();
	float followSpeed = PLAYER_SPEED;
	mTargetX = mTargetTryLanes;
	mTargetY = pPlayer->GetPosition().y;
	pPlayer->GoToPosition(mTargetX, mTargetY, PLAYER_SPEED);
}

void RugbyPlayerAction_Support::OnUpdate(Player* pPlayer)
{
	if (!mballCarrier || mballCarrier == pPlayer) return;
	float safeDistance = 50.0f;
	sf::Vector2f playerPos = pPlayer->GetPosition();
	sf::Vector2f carrierPos = mballCarrier->GetPosition();
	if (carrierPos.x < mTargetX && playerPos.x < carrierPos.x &&
		Utils::GetDistance(playerPos.x, playerPos.y, carrierPos.x, carrierPos.y) > safeDistance)
	{
		pPlayer->SetSpeed(PLAYER_SPEED);
		if (pPlayer->GoToPosition(mTargetX, mTargetY, mfollowSpeed)) {
			return;
		}
	}
	else
	{
		mfollowSpeed*=0.7;
		pPlayer->SetSpeed(mfollowSpeed);
	}
}

void RugbyPlayerAction_Support::OnEnd(Player* pPlayer)
{

}
