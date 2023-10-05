#include "NoaEngine/NoaEngine.h"

using namespace noa;
using namespace std;

class Bullet :public Actor 
{
public:
	NOBJECT(Bullet)
private:
	Bullet(Scene* scene) :Actor(scene) 
	{
		spriteRenderer->SetSprite(&sprite);
	}
public:

public:

	void Start() override {

	}

	void Update() override {
		rigid->velocity.y = -10;
		time = time + Time::deltaTime;
		if (time>0.52)
		{
			Destroy();
		}
	}

	void SetPostion(const Vector<float> & pos)
	{
		this->transform.position = pos;
	}
private:
	Rigidbody* rigid = Rigidbody::Create(this);
	SpriteRenderer* spriteRenderer = SpriteRenderer::Create(this);
	Sprite sprite = Sprite(resource.LoadSprFile("./Assets/Fly/bullet.spr"), { pixelWidth / 10,pixelWidth / 10 });
	float time = 0;
};

class Player :public Actor {
	NOBJECT(Player)
private:
	Player(Scene * scene) :Actor(scene)
	{
		spriteRenderer->SetSprite(&sprite);
		rigid->useGravity = false;
	}

public:

	void Update() override {

		rigid->velocity.x = 0.995f*rigid->velocity.x;
		if (inputSystem.GetKeyHold(KeyCode::KEY_A))
		{
			rigid->velocity.x = -7;
		}
		if (inputSystem.GetKeyHold(KeyCode::KEY_D)) 
		{
			rigid->velocity.x = 7;
		}
		if (inputSystem.GetKeyHold(KeyCode::KEY_W))
		{
			rigid->velocity.y = -7;
		}
		if (inputSystem.GetKeyHold(KeyCode::KEY_S))
		{
			rigid->velocity.y = 7;
		}

		if (rigid->velocity.x<0&&transform.position.x<=0)
		{
			rigid->velocity.x = 0;
			transform.position.x = 0;
		}
		else if (rigid->velocity.x > 0 && transform.position.x >= 9)
		{
			rigid->velocity.x = 0;
			transform.position.x = 9;
		}

		if (rigid->velocity.y < 0 && transform.position.y <= 2.5)
		{
			rigid->velocity.y = 0;
			transform.position.y = 2.5;
		}
		else if (rigid->velocity.y > 0 && transform.position.y >= 4.5)
		{
			rigid->velocity.y = 0;
			transform.position.y = 4.5;
		}

		if (inputSystem.GetKeyHold(KeyCode::KEY_K)) 
		{
			
			delay = delay + Time::deltaTime;
			if (delay>0.05)
			{
				Bullet* bullet = NObject<Bullet>::Create<Scene*>(this->activeScene);
				bullet->SetPostion(this->transform.position);
				delay = 0;
			}
		}

	}

public:
	SpriteRenderer* spriteRenderer = SpriteRenderer::Create(this);
	Rigidbody* rigid = Rigidbody::Create(this);
	CircleCollider2D* collider = CircleCollider2D::Create(this,rigid);

private:
	Sprite sprite = Sprite(resource.LoadSprFile("./Assets/Fly/player.spr"), { pixelWidth/10,pixelWidth/10});
	float delay = 0;
};

class SimpleScene :public Scene {
private:
	SimpleScene() :Scene("SimpleScene")
	{
		camera = new StaticCamera(&background);
		camera->SetTileScale({pixelWidth/10,pixelWidth/10});
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
		PhysicsSystem::SetGrid(10, 10*pixelHeight/(pixelWidth));
		player = NObject<Player>::Create<Scene*>(this);
		player->transform.position = { 4.5f,4.5f };
	}

	void Update() override {

		camera->Render();

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
		//inputSystem.SetRelativeMouseMode(true);
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
	TestGPU test(1920/2, 1080/2, WindowMode::WINDOW, "TestGPU");
	test.Run();
	return 0;
}