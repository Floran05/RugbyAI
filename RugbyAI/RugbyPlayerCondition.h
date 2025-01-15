#pragma once

#include "Condition.h"
#include "Player.h"

class RugbyPlayerCondition_HasBall : public Condition<Player>
{
public:
	bool OnTest(Player* owner) override;
};
