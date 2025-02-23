#pragma once

#include "Entity.h"

class Player;

class Ball : public Entity
{
protected:

	virtual void OnInitialize() override;
	virtual void OnUpdate() override;
	virtual void OnCollision(Entity* collidedWith) override;
	virtual void OnDestroy() override;

protected:

	Player* mOwner;
	Player* mTarget;

public:

	void SetOwner(Player* owner) { mOwner = owner; }
	Player* GetOwner() const { return mOwner; }

	void SetTarget(Player* target) { mTarget = target; }
	Player* GetTarget() const { return mTarget; }

	void TeleportToOwner();

};

