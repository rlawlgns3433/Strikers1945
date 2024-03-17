#include "pch.h"
#include "AniTest.h"

AniTest::AniTest(const std::string& name)
	: SpriteGo(name)
{
}

void AniTest::Init()
{
	SpriteGo::Init();
	animator.SetTarget(&sprite);
}

void AniTest::Reset()
{
	animator.Play("animations/Idle.csv");
	SetOrigin(Origins::BC);
	SetPosition({ 0.f, 0.f });

	animator.ClearEvent();

	std::function<void()> funcInstance = std::bind(&AniTest::TestInstance, this);  
	animator.AddEvent("animations/Jump.csv", 5, funcInstance);

	std::function<void()> funcStatic = std::bind(&AniTest::TestStatic);
	animator.AddEvent("animations/Idle.csv", 5, funcStatic);
}

void AniTest::Update(float dt)
{
	SpriteGo::Update(dt);
	animator.Update(dt);

	float h = InputManager::GetAxisRaw(Axis::Horizontal);

	if (isGrounded && InputManager::GetKeyDown(sf::Keyboard::Space))
	{
		isGrounded = false;
		animator.Play("animations/Jump.csv");
		velocity.y = -500.f;
	}

	velocity.x = h * speed;
	velocity.y += gravity * dt;

	position += velocity * dt;

	if (position.y > 0.f)
	{
		isGrounded = true;
		position.y = 0.f;
		velocity.y = 0.f;
	}

	SetPosition(position);

	if (h != 0.f)
	{
		SetFlipX(h < 0);
	}

	if (animator.GetCurrentClipId() == "animations/Idle.csv")
	{
		if (h != 0.f)
		{
			animator.Play("animations/Run.csv");
		}
	}
	else if (animator.GetCurrentClipId() == "animations/Run.csv")
	{
		if (h == 0.f)
		{
			animator.Play("animations/Idle.csv");
		}
	}
	else if (isGrounded && animator.GetCurrentClipId() == "animations/Jump.csv")
	{
		if (h == 0.f)
		{
			animator.Play("animations/Idle.csv");
		}
		else
		{
			animator.Play("animations/Run.csv");
		}
	}
}

void AniTest::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void AniTest::TestInstance()
{
	std::cout << "TestIntance" << std::endl;
}

void AniTest::TestStatic()
{
	std::cout << "TestStatic" << std::endl;
}
