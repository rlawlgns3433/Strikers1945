#pragma once

namespace Utils
{
	class Random
	{
	public:
		static float RandomValue(); // 0.0 ~ 1.0
		static float RandomRange(float min, float max);
		static sf::Vector2f RandomOnUnitCircle();
		static sf::Vector2f RandomInUnitCircle();
		static sf::Vector2f GetRandomVector2(float start, float end);
		static float GetRandomAngle();
	};
}