#pragma once

#include <SFML/System/Vector2.hpp>

namespace Utils
{
	bool Normalize(sf::Vector2f& vector);
	float GetDistance(int x1, int y1, int x2, int y2);
	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2);

	float DistanceFromPointToLine(float a, float b, float c, int x, int y);
	sf::Vector2f ClosestPointOnSegment(int x1, int y1, int x2, int y2, int px, int py);
	float Lerp(float a, float b, float t);
	sf::Vector2f RotateVector(const sf::Vector2f& v, float degrees);

}