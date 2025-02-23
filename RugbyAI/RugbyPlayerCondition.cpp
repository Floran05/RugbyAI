#include "RugbyPlayerCondition.h"
#include "RugbyScene.h"
#include "Ball.h"
#include "Utils.h"

bool RugbyPlayerCondition_AllyHasBall::OnTest(Player* player)
{
	RugbyScene* scene = player->GetScene<RugbyScene>();
	Ball* ball = scene->GetBall(); 
	if (!ball->GetOwner()) return false;
	return ball->GetOwner()->GetTag() == player->GetTag();
}

bool RubgyPlayerCondition_ShouldPass::OnTest(Player* player)
{
    RugbyScene* scene = player->GetScene<RugbyScene>();
    Ball* ball = scene->GetBall();
    if (!player->HasBall()) return false;
    Player** players = scene->GetPlayers();
    for (int i = 0; i < 10; ++i) {
        if (!players[i]) continue;
        if (players[i]->IsTag(player->GetTag())) continue;
        float distance = Utils::GetDistance(
            player->GetPosition().x, player->GetPosition().y,
            players[i]->GetPosition().x, players[i]->GetPosition().y
        );

        if (distance <= OPPONENT_DISTANCE) {
            return true; 
        }
    }
    return false;
}

bool RugbyPlayerCondition_HasBall::OnTest(Player* owner)
{
	return owner->HasBall();
}

bool RugbyPlayerCondition_HasPlayerAbleToReceivePass::OnTest(Player* owner)
{
    RugbyScene* scene = owner->GetScene<RugbyScene>();
    return scene->GetBestTeammateForPass() != nullptr;
}
