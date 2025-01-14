#include "RugbyScene.h"
#include "Debug.h"
#include "Player.h"
#include <iostream>
#define ZONE_COUNT 3
void RugbyScene::OnInitialize()
{
	int width = GetWindowWidth();
	int height = GetWindowHeight();
	float zoneHeight = height / (ZONE_COUNT*2.f - 2);
	float zoneWidt = width;
	for (int i = 0; i < ZONE_COUNT; ++i) {
		int yMin = i * zoneHeight;
		int yMax = yMin + zoneHeight*2;
		mAreas[i] = { 0,width,yMin,yMax };
	}
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
	for (int i = 0; i < ZONE_COUNT; i++)
	{
		const Box& box = mAreas[i];

		Debug::DrawRectangle(box.xMin, box.yMin, box.width, box.height, mColors[i]);
	}
	
}
