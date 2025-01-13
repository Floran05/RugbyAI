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

	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;

};

