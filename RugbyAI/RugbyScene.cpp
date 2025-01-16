#include "RugbyScene.h"
#include "Debug.h"
#include "Player.h"
#include "Ball.h"
#include "Utils.h"

#include <iostream>
#include <limits>
#include <time.h>

void RugbyScene::OnInitialize()
{
	srand(time(NULL));

	GreenTeamPoints = 0;
	RedTeamPoints = 0;

	// Create areas
	int width = GetWindowWidth();
	int height = GetWindowHeight();
	float zoneHeight = height / (ZONE_COUNT * 2.f - 2);
	float zoneWidt = width;
	for (int i = 0; i < ZONE_COUNT; ++i) {
		int yMin = i * zoneHeight;
		int yMax = yMin + zoneHeight * 2;
		mAreas[i] = { 0, width, yMin, yMax };
	}

	// Init teams
	CreateTeam(true, sf::Color::Green);  
	SetPlayerPositions(true);
	CreateTeam(false, sf::Color::Red);  
	SetPlayerPositions(false);

	// Create ball
	mBall = CreateEntity<Ball>(BALL_RADIUS, sf::Color(240, 95, 64));
	mBall->SetPosition(width * 0.5f, height * 0.5f);

	// Give ball to random player
	GiveBallToPlayer(mPlayers[rand() % 10]);
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

		if (event.key.code == sf::Keyboard::Space && mBall->GetOwner())
		{
			if (Player* target = GetBestTeammateForPass())
			{
				mBall->GetOwner()->PassBall(target);
			}
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
	Debug::DrawText(windowWidth * 0.5f, 10.f, std::to_string(GreenTeamPoints) + " - " + std::to_string(RedTeamPoints), 0.5f, 0.5f, sf::Color::White);
	// Show selected player
	if (mSelectedPlayer)
	{
		const sf::Vector2f position = mSelectedPlayer->GetPosition();
		Debug::DrawOutlinedCircle(position.x, position.y, BALL_RADIUS + 2.f, mSelectedPlayer->GetRadius() - 10.f - BALL_RADIUS, sf::Color(248, 49, 242)); // Pink
	}
	// Draw pass
	if (mBall->GetOwner())
	{
		const std::vector<TargetPassStatus> targets = GetTeammatesPassStatus();
		float maxScore = targets.size() ? targets[0].score : -1.f;
		int maxScoreIndex = targets.size() ? 0 : -1;
		int index = 0;
		for (const TargetPassStatus& target : targets)
		{
			if (target.score > maxScore)
			{
				maxScore = target.score;
				maxScoreIndex = index;
			}

			const sf::Vector2f targetPosition = target.target->GetPosition();
			switch (target.status)
			{
			case PassStatus::AllConditionsGood:
				Debug::DrawOutlinedCircle(targetPosition.x, targetPosition.y, target.target->GetRadius(), 4.f, sf::Color(11, 120, 211)); // Blue
				break;
			case PassStatus::TooFar:
				Debug::DrawOutlinedCircle(targetPosition.x, targetPosition.y, target.target->GetRadius(), 4.f, sf::Color(236, 236, 25)); // Yellow
				break;
			case PassStatus::NearOpponent:
				Debug::DrawOutlinedCircle(targetPosition.x, targetPosition.y, target.target->GetRadius(), 4.f, sf::Color(211, 42, 11)); // Red
				break;
			case PassStatus::OpponentInPath:
				Debug::DrawOutlinedCircle(targetPosition.x, targetPosition.y, target.target->GetRadius(), 4.f, sf::Color(151, 11, 211)); // Purple
				break;
			}

			++index;
		}

		if (maxScoreIndex >= 0)
		{
			const sf::Vector2f targetPosition = targets[maxScoreIndex].target->GetPosition();
			Debug::DrawOutlinedCircle(targetPosition.x, targetPosition.y, targets[maxScoreIndex].target->GetRadius() - 8.f, 8.f, sf::Color(255, 159, 241)); // Light pink
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
			pPlayer->SetIndex(playerIndex % 5);

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
	mBall->SetTarget(nullptr);
	mBall->SetOwner(targetPlayer);
	mBall->TeleportToOwner();
}

std::vector<TargetPassStatus> RugbyScene::GetTeammatesPassStatus()
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
		float distanceScore = 0.f;
		float opponentScore = 0.f;
		float interceptionScore = 0.f;

		// Check if ahead of carrier
		if ((player->IsTag(Tag::PlayerGreen) && playerPosition.x > playerWithBallPosition.x) || 
			(player->IsTag(Tag::PlayerRed)	 && playerPosition.x < playerWithBallPosition.x))
		{
			status = PassStatus::AheadCarrier;
		}

		// Check distance with teammate
		const float distanceWithTeammate = Utils::GetDistance(playerWithBallPosition.x, playerWithBallPosition.y, playerPosition.x, playerPosition.y);
		distanceScore = 1.f / (distanceWithTeammate + 1.f);
		if (distanceWithTeammate > MAX_PASS_DISTANCE)
		{
			status = PassStatus::TooFar;
		}

		
		for (Player* opponent : mPlayers)
		{
			if (opponent->IsTag(playerWithBall->GetTag())) continue;
			const sf::Vector2f opponentPosition = opponent->GetPosition();

			// Check receiver opponents distance
			const float receiverOpponentDistance = Utils::GetDistance(playerPosition.x, playerPosition.y, opponentPosition.x, opponentPosition.y);
			opponentScore += 1.f / (receiverOpponentDistance + 1.f);
			if (receiverOpponentDistance < ENEMY_DISTANCE)
			{
				status = PassStatus::NearOpponent;
				break;
			}

			// Check if opponent can intercept
			interceptionScore = InterceptionRisk(playerWithBallPosition.x, playerWithBallPosition.y, playerPosition.x, playerPosition.y, opponentPosition.x, opponentPosition.y);
			interceptionScore += interceptionScore;
			if (interceptionScore > 0)
			{
				status = PassStatus::OpponentInPath;
				break;
			}
		}
		opponentScore = 1.f / (opponentScore + 1.f);
		const float score = distanceScore + opponentScore - 3.f * interceptionScore;
		eligiblePlayers.emplace_back(TargetPassStatus{player, status, score});
	}

	return eligiblePlayers;
}

Player* RugbyScene::GetBestTeammateForPass()
{
	std::vector<TargetPassStatus> statuses = GetTeammatesPassStatus();
	TargetPassStatus bestStatus{ nullptr, PassStatus::OpponentInPath, -1.f };
	for (const TargetPassStatus& status : statuses)
	{
		if (status.status == PassStatus::AllConditionsGood && status.score > bestStatus.score)
		{
			bestStatus = status;
		}
	}
	return bestStatus.target;
}

float RugbyScene::InterceptionRisk(int senderX, int senderY, int receiverX, int receiverY, int opponentX, int opponentY)
{
	if (opponentX < std::min((int)(senderX - PLAYER_RADIUS), (int)(receiverX - PLAYER_RADIUS))) return 0.f;
	if (opponentX > std::max((int)(senderX + PLAYER_RADIUS), (int)(receiverX + PLAYER_RADIUS))) return 0.f;
	if (opponentY < std::min((int)(senderY - PLAYER_RADIUS), (int)(receiverY - PLAYER_RADIUS))) return 0.f;
	if (opponentY > std::max((int)(senderY + PLAYER_RADIUS), (int)(receiverY + PLAYER_RADIUS))) return 0.f;

	const float a = receiverY - senderY;
	const float b = -(receiverX - senderX);
	const float c = -a * senderX - b * senderY;

	const float distanceFromLine = Utils::DistanceFromPointToLine(a, b, c, opponentX, opponentY);

	const float ballDistance = Utils::GetDistance(senderX, senderY, receiverX, receiverY);
	const float ballTravelTime = ballDistance / BALL_SPEED;

	const float opponentTravelTime = distanceFromLine / PLAYER_SPEED;

	return opponentTravelTime <= ballTravelTime ? (1.f / (distanceFromLine + 1.f)) : 0.f;
}

Player* RugbyScene::GetOpponentPlayerByIndex(Player* player, int targetIndex)
{
	if (targetIndex < 0)
	{
		targetIndex = player->GetIndex();
	}

	const bool isGreenTeam = player->IsTag(Tag::PlayerGreen);
	targetIndex = isGreenTeam ? targetIndex + 5 : targetIndex;

	if (targetIndex > 9) return nullptr;

	return mPlayers[targetIndex];
}
