#include "Ball.h"
#include "Player.h"
#include "RugbyScene.h"

void Ball::OnInitialize()
{
	SetTag(RugbyScene::Tag::RugbyBall);
}

void Ball::OnUpdate()
{
	if (mOwner)
	{
		const sf::Vector2f ownerPosition = mOwner->GetPosition();
		SetPosition(ownerPosition.x, ownerPosition.y);
	}
}

void Ball::OnCollision(Entity* collidedWith)
{
}

void Ball::OnDestroy()
{
}
