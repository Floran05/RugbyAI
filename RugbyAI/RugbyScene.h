#pragma once

#include "Scene.h"

namespace sf
{
	class Event;
}

struct Box
{
	int xMin;
	int xMax;
	int yMin;
	int yMax;

	int width = xMax - xMin;
	int height = yMax - yMin;

	constexpr bool IsPointInBox(int x, int y) const {
		return x >= xMin && x <= xMax && y >= yMin && y <= yMax;
	}
};

class RugbyScene : public Scene
{

public:
	
	enum Tag
	{
		PlayerGreen,
		PlayerRed,
		Ball
	};
	Box mAreas[3];
	sf::Color mColors[3] = { sf::Color::Red,sf::Color::Blue,sf::Color::White };
	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;

};

