#include "pch.h"
#include "SoundManager.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	Release();
}

void SoundManager::Init(int totalChannels)
{
	Release();

	for (int i = 0; i < totalChannels; ++i)
	{
		waitingSFX.push_back(new sf::Sound());
	}
}

void SoundManager::Release()
{
	for (auto sound : waitingSFX)
	{
		delete sound;
	}

	waitingSFX.clear();

	for (auto sound : playingSFX)
	{
		delete sound;
	}

	playingSFX.clear();
}

void SoundManager::Update(float dt)
{
	for (auto it = playingSFX.begin(); it != playingSFX.end();)
	{
		if ((*it)->getStatus() != sf::SoundSource::Stopped)
		{
			waitingSFX.push_back(*it);
			it = playingSFX.erase(it); // erase는 삭제한 원소의 위치 다음 iterator가 반환된다.
									   // 그렇기 때문에 이 부분에서는 iterator가 수정되면 안 된다.
		}
		else
		{
			++it;
		}
	}

	if (isFading)
	{
		bool isEndBack = false;
		bool isEndFront = false;

		backBgmIndex = (frontBgmIndex == 1) ? 0 : 1;
		float backVolume = bgm[backBgmIndex].getVolume();
		backVolume = Utils::MyMath::Lerp(backVolume, 0.f, dt * fadeSpeed);
		bgm[backBgmIndex].setVolume(backVolume);
		if (abs(backVolume) < fadeLimit)
		{
			bgm[backBgmIndex].setVolume(0.f);
			bgm[backBgmIndex].stop();
			isEndBack = true;
		}

		float frontVolume = bgm[frontBgmIndex].getVolume();
		frontVolume = Utils::MyMath::Lerp(frontVolume, bgmVolume, dt * fadeSpeed);
		bgm[frontBgmIndex].setVolume(frontVolume);
		if (abs(backVolume - frontVolume) < fadeLimit)
		{
			bgm[frontBgmIndex].setVolume(bgmVolume);
			isEndFront = true;
		}

		if (isEndBack && isEndFront)
		{
			isFading = false;
		}
	}
}

void SoundManager::PlayBgm(const std::string& id, bool crossFade)
{
	isFading = true;
	frontBgmIndex = (frontBgmIndex + 1) % 2;
	backBgmIndex = (frontBgmIndex == 1) ? 0 : 1;
	
	bgm[frontBgmIndex].setBuffer(*SOUND_BUFFER_MANAGER.GetResource(id));
	bgm[frontBgmIndex].setVolume(0.f);
	bgm[frontBgmIndex].setLoop(true);
	bgm[frontBgmIndex].play();

}

void SoundManager::StopBgm()
{
	isFading = false;
	for (int i = 0; i < _countof(bgm); ++i)
	{
		bgm[i].stop();
	}
}

void SoundManager::PlaySfx(const std::string& id, bool loop)
{
	PlaySfx(*SOUND_BUFFER_MANAGER.GetResource(id), loop);
}

void SoundManager::PlaySfx(const sf::SoundBuffer& buffer, bool loop)
{
	sf::Sound* sound = nullptr;

	if (waitingSFX.empty())
	{
		sound = playingSFX.front(); // play 중인 sfx를 중지하고 재사용하기 때문에 플레이 되는 것처럼 보임. (해결 필요)
		playingSFX.pop_front();
		sound->stop();
	}
	else
	{
		sound = waitingSFX.front();
		waitingSFX.pop_front();
	}

	sound->setBuffer(buffer);
	sound->setLoop(loop);
	sound->setVolume(sfxVolume);
	sound->play();
	playingSFX.push_back(sound);
}

void SoundManager::StopAllSfx()
{
	for (auto sfx : playingSFX)
	{
		sfx->stop();
		waitingSFX.push_back(sfx);
	}
	playingSFX.clear();
}
