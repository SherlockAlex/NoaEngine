#include "GameEngine.h"
#include "Player.h"
#include "Enimy.h"
#include "Bullet.h"
#include "BulletFactory.h"
#include "BulletPool.h"
#include "./../NoaEngine/NoaEngine/Stack.h"
#include "./../NoaEngine/NoaEngine/Queue.h"
#include "./../NoaEngine/NoaEngine/List.h"

using namespace std;
using namespace noa;

class SimpleScene :public Scene {
private:
	SimpleScene() :Scene("SimpleScene")
	{
		
	}
public:
	static SimpleScene* Create() {
		return new SimpleScene();
	}

	void Awake() override
	{
		PhysicsSystem::SetGrid(10, 10 * pixelHeight / (pixelWidth));
		
		//初始化Camera
		camera = NObject<StaticCamera>::Create<Scene*>(this);
		camera->SetBackground(&background);
		camera->SetTileScale({ pixelWidth / 10,pixelWidth / 10 });

		player = NObject<Player>::Create<Scene*>(this);
		player->transform.position = { 4.5f,4.5f };

		Enimy* enimy = NObject<Enimy>::Create<Scene*>(this);
		enimy->SetPosition({4.5f,0.0f});

	}

private:
	Sprite background = Sprite(resource.LoadSprFile("./Assets/Fly/background.spr"), { pixelWidth,pixelHeight });

	StaticCamera* camera = nullptr;
	Player* player = nullptr;
};

class Demo :public NoaEngine {
public:
	Demo(int w, int h, WindowMode windowMode, string name)
		:NoaEngine(w, h, windowMode, name)
	{

		scene = SimpleScene::Create();

		sceneManager.LoadScene("SimpleScene");

		bulletFactory = make_shared<BulletFactory>();
		bulletPool = make_shared<BulletPool>();

	}

	void Start() override 
	{
		
		bgm.Play(true);
	}

	void Update() override 
	{
		if (inputSystem.GetKeyHold(KeyCode::KEY_ESC))
		{
			Quit();
		}
	}


public:
	SimpleScene* scene;
	Audio bgm = Audio("./Assets/Fly/Audio/bgm.ogg", AudioType::CHUNK);

};

int main() {
	List<int> queue;
	auto myStartTime = std::chrono::high_resolution_clock::now();
	for (int i = 0;i<10000;i++)
	{
		queue.Add(i);
	}
	auto myEndTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> myCast = myEndTime - myStartTime;
	Debug::Log(ToString<double>(myCast.count()));

	std::vector<int> sysq;
	auto sysStartTime = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++)
	{
		sysq.push_back(i);
	}
	auto sysEndTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> sysCast = sysEndTime - sysStartTime;
	Debug::Log(ToString<double>(sysCast.count()));

	Demo demo(1920 / 2, 1080 / 2, WindowMode::WINDOW, "TestGPU");
	demo.Run();
	return 0;
}