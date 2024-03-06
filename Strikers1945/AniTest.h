#pragma once
#include "SpriteGo.h"
#include "Animator.h"
class AniTest : public SpriteGo
{
protected:
	AniTest(const AniTest&) = delete;
	AniTest(AniTest&&) = delete;
	AniTest& operator=(const AniTest&) = delete;
	AniTest& operator=(AniTest&&) = delete;

	Animator animator;

	sf::Vector2f velocity;

	float gravity = 500.f;
	float speed = 500;

	bool isGrounded = true;

public:
	AniTest(const std::string& name = "");
	~AniTest() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void TestInstance();
	static void TestStatic();
};