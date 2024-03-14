#pragma once
#include "Singleton.h"

class Scene; 

enum class SceneIDs
{
	None = -1,
	SceneTitle,
	SceneGame,
	SceneEnding,
	COUNT,
};

class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;

public:

protected:
	SceneManager(const SceneManager&)			 = delete;
	SceneManager(SceneManager&&)				 = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager& operator=(SceneManager&&)		 = delete;

	std::vector<Scene*> scenes; 
	SceneIDs startScene = SceneIDs::SceneTitle;
	SceneIDs currentScene = startScene;
	SceneIDs nextScene = SceneIDs::None;

	std::vector<std::string> TextureResourceNames = { };
	std::vector<std::string> FontResourceNames = { /*"fonts/DS-DIGI.ttf"*/};
	std::vector<std::string> SoundResourceNames = {};

	std::string textInputBuffer = "";

	unsigned lives = 3;
	unsigned score = 0;

public:

	SceneManager() = default;
	virtual ~SceneManager();
	
	void Init(); // 의도한 시점에 생성 또는 소멸시킬 수 있도록 함수를 정의
	void Release(); // 의도한 시점에 생성 또는 소멸시킬 수 있도록 함수를 정의

	Scene* GetCurrentScene() { return scenes[(int)currentScene]; }
	Scene* GetScene(SceneIDs id) { return scenes[(int)id]; }
	
	void LoadAllResources();
	void ChangeScene(SceneIDs id);
	bool Update(float dt);
	void UpdateEvent(const sf::Event& event);
	void LateUpdate(float dt);
	void FixeUpdate(float dt);
	void Draw(sf::RenderWindow& window);


	std::string& GetBuffer() { return textInputBuffer; }
	void ClearBuffer();

};

#define SCENE_MANAGER (Singleton<SceneManager>::Instance())