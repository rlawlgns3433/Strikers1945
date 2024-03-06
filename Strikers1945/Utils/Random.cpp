#include "pch.h"
#include "Random.h"

namespace Utils
{
	float Random::RandomValue()
	{
		return (float)rand() / RAND_MAX;
	}

	float Random::RandomRange(float min, float max)
	{
		float value = RandomValue();
		value *= (max - min);
		value += min;

		return value;
	}

	sf::Vector2f Random::RandomOnUnitCircle()
	{
		sf::Transform rotation;
		rotation.rotate(RandomRange(0, 360));
		return rotation * sf::Vector2f{ 1.f, 0.f };
	}

	sf::Vector2f Random::RandomInUnitCircle()
	{
		return RandomOnUnitCircle() * RandomValue();
	}

	sf::Vector2f Random::GetRandomVector2(float start, float end)
	{
		float randVal = (rand() % ((int)(end * 100) - (int)(start * 100))) + start * 100;
		return sf::Vector2f(randVal / 100, randVal / 100);
	}

	float Random::GetRandomAngle()
	{
		return (float)rand() / RAND_MAX * 360.f;
	}
}