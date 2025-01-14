#pragma once

#include "Entity.h"
#include "Ball.h"

class Box;

class Player : public Entity
{

protected:

	virtual void OnInitialize() override;
	virtual void OnUpdate() override;
	virtual void OnCollision(Entity* collidedWith) override;
	virtual void OnDestroy() override;

protected:

	Ball* mBall;
	Box* mDisplacementBoundingBox;

public:

	virtual void SetDisplacementBoundingBox(Box* displacementBox) { mDisplacementBoundingBox = displacementBox; };
	virtual Box* GetDisplacementBoundingBox() const { return mDisplacementBoundingBox; }

	virtual void PassBall(Player* targetPlayer);
	virtual void RecoverBall(Ball* ball);

};

