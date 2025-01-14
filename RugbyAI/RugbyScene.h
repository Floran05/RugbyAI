#pragma once

#include "Scene.h"

namespace sf
{
	class Event;
}

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

