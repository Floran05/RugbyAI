#include "Ball.h"
#include "Player.h"
#include "RugbyScene.h"

#include <iostream>

void Ball::OnInitialize()
{
	SetTag(RugbyScene::Tag::RugbyBall);

	mOwner = nullptr;
	mTarget = nullptr;
}

void Ball::OnUpdate()
{
	if (mTarget)
	{
		const sf::Vector2f targetPosition = mTarget->GetPosition();
		GoToPosition(targetPosition.x, targetPosition.y, BALL_SPEED);
		return;
	}

	if (mOwner)
	{
		const sf::Vector2f ownerPosition = mOwner->GetPosition();
		SetPosition(ownerPosition.x, ownerPosition.y);
	}
}

void Ball::OnCollision(Entity* collidedWith)
{
	if (collidedWith == nullptr || collidedWith == mOwner) return;

	if (Player* player = dynamic_cast<Player*>(collidedWith))
	{
		mTarget = nullptr;
		mOwner = player;
		TeleportToOwner();
	}
}

void Ball::OnDestroy()
{
}

void Ball::TeleportToOwner()
{
	if (mOwner == nullptr) return;
	
	const sf::Vector2f position = mOwner->GetPosition();
	SetPosition(position.x, position.y);
}
