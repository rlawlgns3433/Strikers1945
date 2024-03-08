#pragma once

namespace Utils
{
	class MyMath
	{
	public:
		static float Magnitude(sf::Vector2f vector); // ±Ê¿Ã
		static float SqrMagnitude(const sf::Vector2f vector); // ±Ê¿Ã ¡¶∞ˆ±Ÿ
		static void Normalize(sf::Vector2f& vector); // ø¯∫ª ∫§≈Õ¿« ±Ê¿Ã∏¶ 1∑Œ πŸ≤€ ∫§≈Õ
		static sf::Vector2f GetNormal(const sf::Vector2f& vector);// ∫§≈Õ¿« ±Ê¿Ã∏¶ 1∑Œ πŸ≤€ ∫§≈Õ
		static float Distance(const sf::Vector2f& src, const sf::Vector2f& dst);
		static float Angle(const sf::Vector2f look);
		static float Rad2Deg(float radian);
		static float Deg2Rad(float degree);
		static float Clamp(float value, float min, float max);
		static sf::Vector2f Clamp(const sf::Vector2f& vector, const sf::FloatRect& rect);
		static sf::FloatRect ResizeRect(const sf::FloatRect& rect, const sf::Vector2f& delta);
		static float Lerp(float min, float max, float deltaTime);

		static void AngleWithDirectionOffsets(const sf::Vector2f& look, sf::Vector2f& directionPlus30, sf::Vector2f& directionMinus30);
	};
}