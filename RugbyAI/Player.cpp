#include "Player.h"
#include "RugbyScene.h"

void Player::OnInitialize()
{
	mInvincibilityDuration = 1.0f;
	mSpeedBoostDuration = 1.5f;
}

void Player::OnUpdate()
{
	if (mDisplacementBoundingBox == nullptr) return;

	const sf::Vector2f topLeftPosition = GetPosition(0.f, 0.f);
	const sf::Vector2f botRightPosition = GetPosition(1.f, 1.f);

	int newPosX = topLeftPosition.x;
	int newPosY = topLeftPosition.y;
	if (topLeftPosition.x < mDisplacementBoundingBox->xMin)
	{
		newPosX = mDisplacementBoundingBox->xMin;
	}
	if (botRightPosition.x > mDisplacementBoundingBox->xMax)
	{
		newPosX = mDisplacementBoundingBox->xMax - (botRightPosition.x - topLeftPosition.x);
	}
	if (topLeftPosition.y < mDisplacementBoundingBox->yMin)
	{
		newPosY = mDisplacementBoundingBox->yMin;
	}
	if (botRightPosition.y > mDisplacementBoundingBox->yMax)
	{
		newPosY = mDisplacementBoundingBox->yMax - (botRightPosition.y - topLeftPosition.y);
	}
	SetPosition(newPosX, newPosY, 0.f, 0.f);
}

void Player::OnCollision(Entity* collidedWith)
{
}

void Player::OnDestroy()
{
}

void Player::PassBall(Player* targetPlayer)
{
	if (mBall == nullptr || targetPlayer == this) return;

	targetPlayer->RecoverBall(mBall);
	mBall = nullptr;
}

void Player::RecoverBall(Ball* ball)
{
	if (mBall) return;

	mBall = ball;
	mBall->SetOwner(this);
}
