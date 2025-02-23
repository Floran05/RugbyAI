#pragma once

#include "Condition.h"
#include "Player.h"

class RugbyPlayerCondition_AllyHasBall : public Condition<Player> {

public :
	bool OnTest(Player* player) override;
};
class RubgyPlayerCondition_ShouldPass : public Condition<Player> {
public:
	bool OnTest(Player* player) override;
};

class RugbyPlayerCondition_HasBall : public Condition<Player>
{
public:
	bool OnTest(Player* owner) override;
};

class RugbyPlayerCondition_HasPlayerAbleToReceivePass : public Condition<Player>
{
public:
	bool OnTest(Player* owner) override;
};