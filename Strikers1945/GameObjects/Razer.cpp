#include "pch.h"
#include "Razer.h"
#include "SceneGame.h"

Razer::Razer(const std::string& name)
	: SpriteGo(name)
{
}

void Razer::Init()
{
	SpriteGo::Init();
	animator.SetTarget(&sprite);

}

void Razer::Reset()
{	
	SetOrigin(Origins::BC);
	animator.Play("animation/Enemy/Razer.csv");
}

void Razer::Update(float dt)
{
	animator.Update(dt);

	if (animator.GetCurrentClipId() != "animation/Enemy/Razer.csv")
	{
		animator.Play("animation/Enemy/Razer.csv");
	}
}

void Razer::Stop()
{
	animator.Stop();
}