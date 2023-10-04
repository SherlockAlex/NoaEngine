#include "NoaEngine/NoaEngine.h"

using namespace noa;
using namespace std;

class Player :public Actor {
private:
	Player(Scene * scene) :Actor(scene)
	{
		spriteRenderer->SetSprite(&sprite);
	}

public:
	static Player* Create(Scene * scene) 
	{
		return new Player(scene);
	}

	void Delete() {
		delete this;
	}

	void Update() override {
		
	}

public:
	SpriteRenderer* spriteRenderer = SpriteRenderer::Create(this);
	Sprite sprite = Sprite(resource.LoadSprFile("./Assets/Fly/player.spr"), { pixelWidth/10,pixelWidth/10});

};

class SimpleScene :public Scene {
private:
	SimpleScene() :Scene("SimpleScene")
	{
		camera = new StaticCamera(&background);
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
		player = Player::Create(this);
	}

	void Update() override {

		camera->Render();
		Vector<double> pos = inputSystem.GetMousePosition();
		player->transform.position.x = pos.x;
		player->transform.position.y = pos.y;

	}

private:
	StaticCamera* camera = nullptr;
	Sprite background = Sprite(resource.LoadSprFile("./Assets/Fly/background.spr"), {pixelWidth,pixelHeight});
	
	Player* player = nullptr;
};

class TestGPU :public NoaEngine {
public:
	TestGPU(int w,int h,WindowMode windowMode,string name) 
	:NoaEngine(w,h,windowMode,name)
	{

		scene = SimpleScene::Create();

		sceneManager.LoadScene("SimpleScene");

	}

	void Start() override {
		inputSystem.SetRelativeMouseMode(true);
	}

	void Update() override {

		

		if (inputSystem.GetKeyHold(KeyCode::KEY_ESC))
		{
			Quit();
		}

	}


public:
	SimpleScene* scene;

};

int main() {
	TestGPU test(1920, 1080, WindowMode::FULLSCREEN, "TestGPU");
	test.Run();
	return 0;
}