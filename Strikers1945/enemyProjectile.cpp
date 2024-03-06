#include "pch.h"
#include "enemyProjectile.h"

enemyProjectile::enemyProjectile(const std::string& name)
	: SpriteGo(name)
{
}

void enemyProjectile::Init()
{
	SpriteGo::Init();
	SetTexture("graphics/Strikers1945/enemyProjectile.png");
	SetTextureRect({ 7, 20, 9, 16 });					 // Rect ���ϱ�
	SetOrigin(Origins::BC);
}

void enemyProjectile::Reset()
{
	SpriteGo::Reset();
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MANAGER.GetCurrentScene());
	player = dynamic_cast<AnimPlayer*>(sceneGame->FindGameObject("player"));
}

void enemyProjectile::Update(float dt)
{
	SpriteGo::Update(dt);
	// �߻� ������ Enemy����
	// �� �Ѿ��� ���⼭ �̵�

}
