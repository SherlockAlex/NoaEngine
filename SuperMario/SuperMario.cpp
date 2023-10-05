#include "GameEngine.h"
#include "Player.h"

using namespace std;
using namespace noa;

class SimpleScene :public Scene {
private:
	SimpleScene() :Scene("SimpleScene")
	{
		camera = new StaticCamera(&background);
		camera->SetTileScale({ pixelWidth / 10,pixelWidth / 10 });
	}
public:
	static SimpleScene* Create() {
		return new SimpleScene();
	}

	void Delete() override {
		delete this;
	}

	void Awake() override
	{
		PhysicsSystem::SetGrid(10, 10 * pixelHeight / (pixelWidth));
		player = NObject<Player>::Create<Scene*>(this);
		player->transform.position = { 4.5f,4.5f };
	}

	void Update() override {

		camera->Render();

	}

private:
	StaticCamera* camera = nullptr;
	Sprite background = Sprite(resource.LoadSprFile("./Assets/Fly/background.spr"), { pixelWidth,pixelHeight });

	Player* player = nullptr;
};

class TestGPU :public NoaEngine {
public:
	TestGPU(int w, int h, WindowMode windowMode, string name)
		:NoaEngine(w, h, windowMode, name)
	{

		scene = SimpleScene::Create();

		sceneManager.LoadScene("SimpleScene");

	}

	void Start() override {
		
		bgm.Play(true);
	}

	void Update() override {

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
	TestGPU test(1920 / 2, 1080 / 2, WindowMode::WINDOW, "TestGPU");
	test.Run();
	return 0;
}