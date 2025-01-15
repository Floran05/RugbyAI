#pragma once

#include "Condition.h"
#include "Player.h"
class PlayerCondition_AllyHasBall : public Condition<Player> {

public :
	bool OnTest(Player* player) override;
};

