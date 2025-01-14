#pragma once

#include "Scene.h"
#include "Player.h"

#define ZONE_COUNT 3
#define TRY_LANES_SCREEN_PERCENT 0.9f
#define PLAYER_SPEED 150.f

class Ball;
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
		RugbyBall
	};

	void OnInitialize() override;
	void OnEvent(const sf::Event& event) override;
	void OnUpdate() override;

protected:

	Box mAreas[3];
	sf::Color mColors[3] = { sf::Color::Red,sf::Color::Blue,sf::Color::White };
	Ball* mBall;
	Player* mPlayers[10];

	Player* mSelectedPlayer;

	int GreenTeamPoints;
	int RedTeamPoints;

public:

	void CreateTeam(bool isLeft, const sf::Color& color);
	void OnGoal(const Tag team);

};

