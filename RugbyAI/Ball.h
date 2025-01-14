#pragma once

#include "Entity.h"

class Ball : public Entity
{
protected:

	virtual void OnInitialize() override;
	virtual void OnUpdate() override;
	virtual void OnCollision(Entity* collidedWith) override;
	virtual void OnDestroy() override;

protected:

	Entity* mOwner;

public:

	void SetOwner(Entity* owner) { mOwner = owner; }

};

