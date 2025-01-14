#include "RugbyScene.h"
#include "Debug.h"

#include <iostream>

void RugbyScene::OnInitialize()
{
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
}
