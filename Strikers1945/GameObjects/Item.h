#pragma once
#include "SpriteGo.h"
#include "Animator.h"

class AnimPlayer;
class SceneGame;

class Item : public SpriteGo
{
public :
	enum class Types
	{
		Life,
		Gold,
		PowerUp,
		Bomb,
	};

	enum class MovementType
	{
		NotMove,
		Move
	};
	static Item* Create(Types itemType);

protected:
	Item(const Item&) = delete;
	Item(Item&&) = delete;
	Item& operator=(const Item&) = delete;
	Item& operator=(Item&&) = delete;

	Types type;
	MovementType movement;

	Animator animator;
	AnimPlayer* player = nullptr;
	SceneGame* sceneGame = nullptr;
	AnimationClip currentClip;

	std::string animationClipId;

	sf::Vector2f direction = { 1.f, 0.f };

	float speed = 300.f;

public:
	Item(const std::string& name = "");
	~Item() override = default;

	void Init() override;
	void Reset() override;
	void Update(float dt) override;
	void LateUpdate(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

