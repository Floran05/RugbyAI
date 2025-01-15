#include "RugbyPlayerCondition.h"

bool RugbyPlayerCondition_HasBall::OnTest(Player* owner)
{
	return owner->HasBall();
}
