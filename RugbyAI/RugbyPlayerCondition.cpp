#include "RugbyPlayerCondition.h"
#include "RugbyScene.h"
#include "Ball.h"
#include "Utils.h"
bool PlayerCondition_AllyHasBall::OnTest(Player* player)
{
	RugbyScene* scene = player->GetScene<RugbyScene>();
	Ball* ball = scene->GetBall(); 
	if (!ball->GetOwner()) return false;
	return ball->GetOwner()->GetTag() == player->GetTag();
}

bool PlayerCondition_ShouldPass::OnTest(Player* player)
{
    RugbyScene* scene = player->GetScene<RugbyScene>();
    Ball* ball = scene->GetBall();
    if (!ball->GetOwner() || ball->GetOwner() != player) return false;
    Player** players = scene->GetPlayers();
    for (int i = 0; i < 10; ++i) {
        if (!players[i]) continue;
        if (players[i]->IsTag(player->GetTag())) continue;
        float distance = Utils::GetDistance(
            player->GetPosition().x, player->GetPosition().y,
            players[i]->GetPosition().x, players[i]->GetPosition().y
        );

        if (distance <= ENEMY_DISTANCE) {
            return true; 
        }
    }
    return false;
}
