#include "core/NoaEngine.h"

using namespace noa;
using namespace std;

class Player :public Actor {
private:
	Player(Scene * scene) :Actor(scene)
	{
		animation = Animation::Create(this,7,true);
		animation->SetFrame(&frame);

		sprite = Sprite(animation->GetCurrentFrameImage(), {pixelWidth / 2,pixelWidth / 2});

		spriteGPU = new SpriteGPU(&sprite);

	}

public:
	static Player* Create(Scene * scene) 
	{
		return new Player(scene);
	}

	void Delete() {
		delete this;
	}

	float eulerAngle = 0.0;

	void Update() override {
		Vector<float> pos;
		pos.x = static_cast<float>(inputSystem.GetMousePosition().x);
		pos.y = static_cast<float>(inputSystem.GetMousePosition().y);

		//Debug(to_string(pos.x) + ":" + to_string(pos.y));

		if (inputSystem.GetMouseButton(MouseButton::LEFT_BUTTON)) 
		{
			eulerAngle += (100*Time::deltaTime);
		}
		else if (inputSystem.GetMouseButton(MouseButton::RIGHT_BUTTON))
		{
			eulerAngle -= (100*Time::deltaTime);
		}

		animation->Play();
		sprite.UpdateImage(animation->GetCurrentFrameImage());
		//sprite.DrawSprite(pos.x, pos.y, true,true);
		spriteGPU->DrawSprite(pos.x, pos.y, true,eulerAngle);
		Debug(to_string(1.0/Time::deltaTime));
		//spriteGPU.DrawSprite(0.5 * pixelWidth, pixelHeight - sprite.scale.y, true);
		//renderer->DrawString("FPS:" + to_string(1.0 / Time::deltaTime), 10, 10, WHITE, 0.05 * pixelWidth);
	}

public:
	Animation* animation = nullptr;

	AnimationFrame frame = AnimationFrame("./Assets/JumpMan/gun-shot.amt");

	Sprite sprite;

	SpriteGPU* spriteGPU;

};

class SimpleScene :public Scene {
private:
	SimpleScene() :Scene("SimpleScene")
	{

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
		Sprite sprite = Sprite(resource.LoadSprFile("./Assets/JumpMan/Texture/gameBackground.spr"), {pixelWidth,pixelHeight});
		sprite.DrawSpriteFull();
	}

	void Update() override {
		//renderer->FullScreen(BLACK);
		player->Update();
	}

private:
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

	}

	void Update() override {

	}


public:
	SimpleScene* scene;

};

int main() {
	TestGPU test(1920/2, 1080/2, WindowMode::WINDOW, "TestGPU");
	test.Run();
	return 0;
}