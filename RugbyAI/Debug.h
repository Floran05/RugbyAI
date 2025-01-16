#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>

class GameManager;

namespace sf
{
	class RenderWindow;
}

struct Line 
{
	sf::Vertex start;
	sf::Vertex end;
};

class Debug
{
	std::vector<Line> mLines;
	std::vector<sf::Text> mTexts;
	std::vector<sf::CircleShape> mCircles;

	void Draw(sf::RenderWindow* pRenderWindow);

public:
	static Debug* Get();

	static void DrawLine(float x1, float y1, float x2, float y2, const sf::Color& color);
	static void DrawRectangle(float x, float y, float width, float height, const sf::Color& color);
	static void DrawCircle(float x, float y, float radius, const sf::Color& color);
	static void DrawOutlinedCircle(float x, float y, float radius, float thickness, const sf::Color& color);
	static void DrawText(float x, float y, const std::string& text, const sf::Color& color, float outlineThickness = 0.f, const sf::Color& outlineColor = sf::Color::Black);
	static void DrawText(float x, float y, const std::string& text, float ratioX, float ratioY, const sf::Color& color, float outlineThickness = 0.f, const sf::Color& outlineColor = sf::Color::Black);

	friend GameManager;
};

