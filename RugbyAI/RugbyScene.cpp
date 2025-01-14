#include "RugbyScene.h"
#include "Debug.h"
#include "Player.h"
#include "Ball.h"

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
}

void RugbyScene::OnEvent(const sf::Event& event)
{
	if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		// Try select player
	}

	// Events on selected player
	if (mSelectedPlayer == nullptr) return;

	if (event.mouseButton.button == sf::Mouse::Button::Left)
	{
		mSelectedPlayer->GoToPosition(event.mouseButton.x, event.mouseButton.y, PLAYER_SPEED);
	}
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
	{
		// Force pass
	}
}

void RugbyScene::OnUpdate()
{
	// Draw goal lines
	int windowWidth = GetWindowWidth();
	int windowHeight = GetWindowHeight();
	Debug::DrawLine(windowWidth * TRY_LANES_SCREEN_PERCENT, 0, windowWidth * TRY_LANES_SCREEN_PERCENT, windowHeight, sf::Color::White);
	Debug::DrawLine(windowWidth * (1.f - TRY_LANES_SCREEN_PERCENT), 0, windowWidth * (1.f - TRY_LANES_SCREEN_PERCENT), windowHeight, sf::Color::White);
	for (int i = 0; i < ZONE_COUNT; ++i)
	{
		const Box& box = mAreas[i];

		Debug::DrawRectangle(box.xMin, box.yMin, box.width, box.height, mColors[i]);
	}
	
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
		Debug::DrawOutlinedCircle(position.x, position.y, mSelectedPlayer->GetRadius(), 2.f, sf::Color::Cyan);
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
}



void RugbyScene::CreateTeam(bool isLeft, const sf::Color& color)
{

	float playerRadius = 30.f;
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
			Player* pPlayer = CreateEntity<Player>(playerRadius, color);

		 
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
	float playerRadius = 30.f;
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
					posY = centerY - playerRadius * 2.f;
				}
				else {
					posY = centerY + playerRadius * 2.f;
				}
			}

			Player* pPlayer = mPlayers[playerIndex++];
			pPlayer->SetPosition(xStart, posY, 0.5f, 0.5f);
			pPlayer->SetDisplacementBoundingBox(&mAreas[zone]);
		}
	}
}