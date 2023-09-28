#include "core/NoaEngine.h"

using namespace noa;
using namespace std;

class Player :public Actor {
private:
	Player(Scene * scene) :Actor(scene)
	{
		animation = Animation::Create(this,7,true);
		animation->SetFrame(&frame);

		sprite = Sprite(animation->GetCurrentFrameImage(), {pixelWidth/2,pixelWidth/2});

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

	void Update() override {
		Vector<float> pos;
		pos.x = inputSystem.GetMousePosition().x;
		pos.y = inputSystem.GetMousePosition().y;

		animation->Play();
		sprite.UpdateImage(animation->GetCurrentFrameImage());
		//sprite.DrawSprite(pos.x, pos.y, true,true);
		spriteGPU->DrawSprite(pos.x, pos.y, true);
		//spriteGPU.DrawSprite(0.5 * pixelWidth, pixelHeight - sprite.scale.y, true);
		renderer.DrawString("FPS:" + to_string(1.0 / deltaTime), 10, 10, WHITE, 0.05 * pixelWidth);
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
	}

	void Update() override {
		renderer.FullScreen(BLACK);
		player->Update();
	}

private:
	Player* player = nullptr;

};

class TestGPU :public NoaEngineGL {
public:
	TestGPU(int w,int h,GameWindowMode windowMode,string name) 
	:NoaEngineGL(w,h,windowMode,name)
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
	TestGPU test(1920/2, 1080/2, NoaEngineGL::WindowMode, "TestGPU");
	test.Run();
	return 0;
}