#include "pch.h"
#include "Bullet.h"

Bullet::Bullet(const std::string& name)
	: SpriteGo(name)
{
}

void Bullet::Init()
{
	SpriteGo::Init();
	SetTexture("graphics/Strikers1945/projectile.png");
	SetTextureRect({7, 20, 9, 16});					 // Rect Á¤ÇÏ±â
	SetOrigin(Origins::BC);
}

void Bullet::Reset()
{
	SpriteGo::Reset();
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetCurrentScene());
	player = dynamic_cast<AnimPlayer*>(sceneGame->FindGameObject("player"));
	damage = 200;
}

void Bullet::Update(float dt)
{
	SpriteGo::Update(dt);
	time += dt;
	Translate(direction * speed * dt);
}