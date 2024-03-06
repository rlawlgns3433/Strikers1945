#pragma once

enum class AnimationLoopType
{
	Single,
	Loop,
	PingPong,
};

struct AnimationFrame
{
	std::string textureId;
	sf::IntRect textureCoord;
	// 필요한 부분 더 추가하기

	AnimationFrame(const std::string& id, const sf::IntRect& coord)
		: textureId(id), textureCoord(coord)
	{
	}

	const sf::Texture& GetTexture() const
	{
		return *TEXTURE_MANAGER.GetResource(textureId);
	}
};

struct AnimationClip
{
	std::string id;
	int fps = 30;
	AnimationLoopType loopType = AnimationLoopType::Single;

	std::vector<AnimationFrame> frames;

	int GetTotalFrame() const
	{
		return frames.size();
	}

	bool loadFromFile(const std::string& filePath);
};

struct AnimationEvent
{
	std::string clipId;
	int frame;
	std::function<void()> action;
};

// 클립의 id를 들고 있다가 재생 (리소스 매니저를 이용)
// 클립의 컨테이너를 들고 있다가 사용
class Animator
{
private:
	Animator(const Animator&) = delete;
	Animator(Animator&&) = delete;
	Animator& operator=(const Animator&) = delete;
	Animator& operator=(Animator&&) = delete;

protected:
	AnimationClip* currentClip = nullptr;

	std::queue<std::string> queue;
	std::list<AnimationEvent> eventList;

	sf::Sprite* target = nullptr; // SpriteGo의 sprite를 가져와서 저장

	float speed = 1.f;
	float clipDuration = 0.f;
	float accumTime = 0.f; // 누적 시간

	int totalFrame = 0;
	int currentFrame = -1; // 전체 프레임 중 현재 어떤 프레임을 재생하고 있는가
	int addFrame = 1;

	bool isPlaying = false;

public:
	Animator();
	virtual ~Animator();

	void Update(float dt);
	void Play(const std::string& clipId, bool clearQueue = true);
	void PlayQueue(const std::string& clipId);
	void Stop();

	//void AddClip(const AnimationClip& clip);
	const std::string& GetCurrentClipId() { return this->currentClip->id; }


	sf::Sprite* GetTarget() const { return this->target; }
	void SetTarget(sf::Sprite* target) { this->target = target; }

	float GetSpeed() const { return this->speed; }
	void SetSpeed(float speed) { this->speed = speed; }

	void SetFrame(const AnimationFrame& frame);

	bool IsPlaying() const { return this->isPlaying; }

	void AddEvent(const std::string& clipId, int frame, std::function<void()> action);
	void ClearEvenet();
};