#include <SFML/System/Vector2.hpp>

#include <cmath>
#include <algorithm>
#include "Utils.h"

namespace Utils 
{
    bool Normalize(sf::Vector2f& vector)
    {
        float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);

		if (magnitude != 0)
		{
			vector.x /= magnitude;
			vector.y /= magnitude;
		
			return true;
		}

		return false;
    }

	float GetDistance(int x1, int y1, int x2, int y2)
	{
		int x = x2 - x1;
		int y = y2 - y1;

		return std::sqrt(x * x + y * y);
	}

	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2)
	{
		float dot = v1.x * v2.x + v1.y * v2.y;
		float det = v1.x * v2.y - v1.y * v2.x;

		return std::atan2(det, dot) * 180 / 3.14159265;
	}
	float Utils::DistanceFromPointToLine(float a, float b, float c, int x, int y)
	{
		return std::abs(a * x + b * y + c) / std::sqrt(a * a + b * b);
	}
	sf::Vector2f ClosestPointOnSegment(int x1, int y1, int x2, int y2, int px, int py)
	{
		float dx = x2 - x1;
		float dy = y2 - y1;

		float vx = px - x1;
		float vy = py - y1;

		float dot = vx * dx + vy * dy;
		float squared = dx * dx + dy * dy;
		float norm = dot / squared;

		norm = std::max(0.f, std::min(1.f, norm));

		float targetX = x1 + norm * dx;
		float targetY = y1 + norm * dy;

		return sf::Vector2f(targetX, targetY);
	}
}