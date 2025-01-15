#pragma once

#include "Condition.h"
#include "Player.h"

class PlayerCondition_AllyHasBall : public Condition<Player> {

public :
	bool OnTest(Player* player) override;
};
class PlayerCondition_ShouldPass : public Condition<Player> {
public:
	bool OnTest(Player* player) override;
};

class RugbyPlayerCondition_HasBall : public Condition<Player>
{
public:
	bool OnTest(Player* owner) override;
};
