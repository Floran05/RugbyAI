#pragma once

#include "Entity.h"
#include "Ball.h"

struct Box
{
	int xMin;
	int xMax;
	int yMin;
	int yMax;

	int width = xMax - xMin;
	int height = yMax - yMin;

	constexpr bool IsPointInBox(int x, int y) const {
		return x >= xMin && x <= xMax && y >= yMin && y <= yMax;
	}
};

class Player : public Entity
{

protected:

	virtual void OnInitialize() override;
	virtual void OnUpdate() override;
	virtual void OnCollision(Entity* collidedWith) override;
	virtual void OnDestroy() override;

protected:

	Ball* mBall;
	Box mDisplacementBoundingBox;

public:

	virtual void SetDisplacementBoundingBox(int xMin, int xMax, int yMin, int yMax);
	virtual Box GetDisplacementBoundingBox() const { return mDisplacementBoundingBox; }

	virtual void PassBall(Player* targetPlayer);
	virtual void RecoverBall(Ball* ball);

};

