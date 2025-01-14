#include "RugbyScene.h"
#include "Debug.h"
#include "Player.h"
#include <iostream>
#define ZONE_COUNT 3
#define PLAYER_COUNT 5
void RugbyScene::OnInitialize()
{
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
	CreateTeam(false, sf::Color::Red);  
}

void RugbyScene::OnEvent(const sf::Event& event)
{
}

void RugbyScene::OnUpdate()
{
	// Draw goal lines
	int windowWidth = GetWindowWidth();
	int windowHeight = GetWindowHeight();
	Debug::DrawLine(windowWidth * 0.1f, 0, windowWidth * 0.1f, windowHeight, sf::Color::White);
	Debug::DrawLine(windowWidth * 0.9f, 0, windowWidth * 0.9f, windowHeight, sf::Color::White);
	/*for (int i = 0; i < ZONE_COUNT; ++i)
	{
		const Box& box = mAreas[i];

		Debug::DrawRectangle(box.xMin, box.yMin, box.width, box.height, mColors[i]);
	}*/
	
}

void RugbyScene::CreateTeam(bool isLeft, sf::Color color)
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

    int playerIndex = 0;

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

            Player* pPlayer = CreateEntity<Player>(playerRadius, color);
            pPlayer->SetPosition(xStart, posY, 0.5f, 0.5f);
            pPlayer->SetDisplacementBoundingBox(&mAreas[zone]);

            if (isLeft) {
                pPlayer->SetTag(RugbyScene::Tag::PlayerGreen);
            }
            else {
                pPlayer->SetTag(RugbyScene::Tag::PlayerRed);
            }

            mPlayers[playerIndex] = pPlayer; 
            playerIndex++;
        }
    }
}
