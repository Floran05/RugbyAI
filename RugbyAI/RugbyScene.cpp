#include "RugbyScene.h"
#include "Debug.h"
#include "Player.h"
#include "Ball.h"
#include "Utils.h"

#include <iostream>

void RugbyScene::OnInitialize()
{
	GreenTeamPoints = 0;
	RedTeamPoints = 0;

	int width = GetWindowWidth();
	int height = GetWindowHeight();
	float zoneHeight = height / (ZONE_COUNT * 2.f - 2);
	float zoneWidt = width;
	for (int i = 0; i < ZONE_COUNT; ++i) {
		int yMin = i * zoneHeight;
		int yMax = yMin + zoneHeight * 2;
		mAreas[i] = { 0, width, yMin, yMax };
	}
	CreateTeam(true, sf::Color::Green);  
	SetPlayerPositions(true);
	CreateTeam(false, sf::Color::Red);  
	SetPlayerPositions(false);
	mBall = CreateEntity<Ball>(15.f, sf::Color(240, 95, 64));
	GiveBallToPlayer(mPlayers[0]);
}

void RugbyScene::OnEvent(const sf::Event& event)
{
	// Mouse events
	if (event.type == sf::Event::EventType::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Button::Right)
		{
			// Try select player
			for (Player* player : mPlayers)
			{
				if (player && player->IsInside(event.mouseButton.x, event.mouseButton.y))
				{
					if (mSelectedPlayer && mSelectedPlayer == player)
					{
						mSelectedPlayer = nullptr;
					}
					else
					{
						mSelectedPlayer = player;
					}
					break;
				}
			}
		}

		// Events on selected player
		if (mSelectedPlayer == nullptr) return;

		if (event.mouseButton.button == sf::Mouse::Button::Left)
		{
			mSelectedPlayer->GoToPosition(event.mouseButton.x, event.mouseButton.y, PLAYER_SPEED);
		}
	}

	if (event.type == sf::Event::KeyPressed)
	{
		// Events on selected player
		if (mSelectedPlayer == nullptr) return;

		if (event.key.code == sf::Keyboard::Space)
		{
			// Force pass
		}
	}
}

void RugbyScene::OnUpdate()
{
	// Draw goal lines
	int windowWidth = GetWindowWidth();
	int windowHeight = GetWindowHeight();
	Debug::DrawLine(windowWidth * TRY_LANES_SCREEN_PERCENT, 0, windowWidth * TRY_LANES_SCREEN_PERCENT, windowHeight, sf::Color::White);
	Debug::DrawLine(windowWidth * (1.f - TRY_LANES_SCREEN_PERCENT), 0, windowWidth * (1.f - TRY_LANES_SCREEN_PERCENT), windowHeight, sf::Color::White);
	/*for (int i = 0; i < ZONE_COUNT; ++i)
	{
		const Box& box = mAreas[i];

		Debug::DrawRectangle(box.xMin, box.yMin, box.width, box.height, mColors[i]);
	}*/
	
	// Check if goal
	if (const Player* owner = mBall->GetOwner())
	{
		if (owner->IsTag(Tag::PlayerGreen))
		{
			const sf::Vector2f position = mBall->GetPosition(0.f);
			if (position.x >= windowWidth * TRY_LANES_SCREEN_PERCENT)
			{
				OnGoal(Tag::PlayerGreen);
			}
		}
		else if (owner->IsTag(Tag::PlayerRed))
		{
			const sf::Vector2f position = mBall->GetPosition(1.f);
			if (position.x <= windowWidth * (1.f - TRY_LANES_SCREEN_PERCENT))
			{
				OnGoal(Tag::PlayerRed);
			}
		}
	}

	// Draw points
	Debug::DrawText(10.f, 10.f, std::to_string(GreenTeamPoints) + " - " + std::to_string(RedTeamPoints), sf::Color::White);
	// Show selected player
	if (mSelectedPlayer)
	{
		const sf::Vector2f position = mSelectedPlayer->GetPosition();
		Debug::DrawOutlinedCircle(position.x, position.y, mSelectedPlayer->GetRadius(), 4.f, sf::Color(248, 49, 242));
		// Draw pass
		if (mBall->GetOwner() && mSelectedPlayer == mBall->GetOwner())
		{
			const std::vector<TargetPassStatus> targets = FindEligiblePlayersForPass();
			for (const TargetPassStatus& target : targets)
			{
				const sf::Vector2f targetPosition = target.target->GetPosition();
				switch (target.status)
				{
				case PassStatus::AllConditionsGood:
					Debug::DrawOutlinedCircle(targetPosition.x, targetPosition.y, target.target->GetRadius(), 4.f, sf::Color(11, 120, 211));
					break;
				case PassStatus::TooFar:
					Debug::DrawOutlinedCircle(targetPosition.x, targetPosition.y, target.target->GetRadius(), 4.f, sf::Color(236, 236, 25));
					break;
				case PassStatus::NearOpponent:
					Debug::DrawOutlinedCircle(targetPosition.x, targetPosition.y, target.target->GetRadius(), 4.f, sf::Color(211, 42, 11));
					break;
				case PassStatus::OpponentInPath:
					Debug::DrawOutlinedCircle(targetPosition.x, targetPosition.y, target.target->GetRadius(), 4.f, sf::Color(151, 11, 211));
					break;
				}
			}
		}
	}
}

void RugbyScene::OnGoal(const Tag team)
{
	switch (team)
	{
	case Tag::PlayerGreen:
		++GreenTeamPoints;
		break;
	case Tag::PlayerRed:
		++RedTeamPoints;
		break;
	}

	SetPlayerPositions(true);
	SetPlayerPositions(false);

	int randomPlayerIndex;
	if (team == Tag::PlayerGreen) {
		randomPlayerIndex = 5 + (rand() % 5);
	}
	else {
		randomPlayerIndex = rand() % 5;
	}
	 
	GiveBallToPlayer(mPlayers[randomPlayerIndex]);
}

void RugbyScene::CreateTeam(bool isLeft, const sf::Color& color)
{
	int playerIndex = isLeft ? 0 : 5;  

	for (int zone = 0; zone < ZONE_COUNT; ++zone) {
		int playerCount;
		if (zone == 1) {
			playerCount = 1;
		}
		else {
			playerCount = 2;
		}

		for (int i = 0; i < playerCount; ++i) {
			Player* pPlayer = CreateEntity<Player>(PLAYER_RADIUS, color);

		 
			if (isLeft) {
				pPlayer->SetTag(RugbyScene::Tag::PlayerGreen);
			}
			else {
				pPlayer->SetTag(RugbyScene::Tag::PlayerRed);
			}

			mPlayers[playerIndex++] = pPlayer;  
		}
	}
}
void RugbyScene::SetPlayerPositions(bool isLeft)
{
	int width = GetWindowWidth();
	float xStart;
	if (isLeft) {
		xStart = width * 0.15f;
	}
	else {
		xStart = width * 0.85f;
	}

	int playerIndex = isLeft ? 0 : 5;  

	for (int zone = 0; zone < ZONE_COUNT; ++zone) {
		int playerCount;
		if (zone == 1) {
			playerCount = 1;
		}
		else {
			playerCount = 2;
		}

		float centerY = (mAreas[zone].yMin + mAreas[zone].yMax) / 2;

		for (int i = 0; i < playerCount; ++i) {
			float posY;
			if (playerCount == 1) {
				posY = centerY;
			}
			else {
				if (i == 0) {
					posY = centerY - PLAYER_RADIUS * 2.f;
				}
				else {
					posY = centerY + PLAYER_RADIUS * 2.f;
				}
			}

			Player* pPlayer = mPlayers[playerIndex++];
			pPlayer->SetPosition(xStart, posY, 0.5f, 0.5f);
			pPlayer->SetDisplacementBoundingBox(&mAreas[zone]);
			pPlayer->SetRigidBody(true);
		}
	}
}

void RugbyScene::GiveBallToPlayer(Player* targetPlayer)
{
	for (Player* player : mPlayers)
	{
		player->PassBall(targetPlayer);
	}
	targetPlayer->RecoverBall(mBall);
}

std::vector<TargetPassStatus> RugbyScene::FindEligiblePlayersForPass()
{
	Player* playerWithBall = mBall->GetOwner();
	if (playerWithBall == nullptr) return std::vector<TargetPassStatus>();
	const sf::Vector2f playerWithBallPosition = playerWithBall->GetPosition();

	std::vector<TargetPassStatus> eligiblePlayers;
	for (Player* player : mPlayers)
	{
		if (player == playerWithBall || !player->IsTag(playerWithBall->GetTag())) continue;
		const sf::Vector2f playerPosition = player->GetPosition();
		PassStatus status = PassStatus::AllConditionsGood;

		// Check distance with teammate
		if (Utils::GetDistance(playerWithBallPosition.x, playerWithBallPosition.y, playerPosition.x, playerPosition.y) > MAX_PASS_DISTANCE)
		{
			status = PassStatus::TooFar;
		}

		for (Player* opponent : mPlayers)
		{
			if (opponent->IsTag(playerWithBall->GetTag())) continue;
			const sf::Vector2f opponentPosition = opponent->GetPosition();

			// Check receiver opponents distance
			if (Utils::GetDistance(playerPosition.x, playerPosition.y, opponentPosition.x, opponentPosition.y) < ENEMY_DISTANCE)
			{
				status = PassStatus::NearOpponent;
				break;
			}

			// Check if opponent can intercept
			if (CanIntercept(playerWithBallPosition.x, playerWithBallPosition.y, playerPosition.x, playerPosition.y, opponentPosition.x, opponentPosition.y))
			{
				status = PassStatus::OpponentInPath;
				break;
			}
		}

		eligiblePlayers.emplace_back(TargetPassStatus{player, status});
	}

	return eligiblePlayers;
}

bool RugbyScene::CanIntercept(int senderX, int senderY, int receiverX, int receiverY, int opponentX, int opponentY)
{
	if (opponentX < std::min((int)(senderX - PLAYER_RADIUS), (int)(receiverX - PLAYER_RADIUS))) return false;
	if (opponentX > std::max((int)(senderX + PLAYER_RADIUS), (int)(receiverX + PLAYER_RADIUS))) return false;
	if (opponentY < std::min((int)(senderY - PLAYER_RADIUS), (int)(receiverY - PLAYER_RADIUS))) return false;
	if (opponentY > std::max((int)(senderY + PLAYER_RADIUS), (int)(receiverY + PLAYER_RADIUS))) return false;

	const float a = receiverY - senderY;
	const float b = -(receiverX - senderX);
	const float c = -a * senderX - b * senderY;

	const float distanceFromLine = Utils::DistanceFromPointToLine(a, b, c, opponentX, opponentY);

	const float ballDistance = Utils::GetDistance(senderX, senderY, receiverX, receiverY);
	const float ballTravelTime = ballDistance / BALL_SPEED;

	const float opponentTravelTime = distanceFromLine / PLAYER_SPEED;

	return opponentTravelTime <= ballTravelTime;
}
