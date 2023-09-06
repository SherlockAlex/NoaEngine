#define Z_ZELDA
#ifdef Z_ZELDA

#include "./core/NoaEngine.h"

using namespace noa;

Vector<int> tileScale(64, 64);

class Test :public GameObject, public Rigidbody {
public:
	Transform* target = nullptr;

	Test() :GameObject(new Sprite(LoadSprFile("./Assets/Zelda/JumpMan.spr"), tileScale)),
		Rigidbody(&transform) {
		useGravity = false;
		//damping = 0.02;
		transform.position = Vector<float>(2,2);

	}

	void Start() override {
		AddForce(Vector<float>(10,0),Rigidbody::Impulse);
		//velocity.x = 10;
	}

	void Update() override 
	{
		//velocity.x = 2;
		//sprite->DrawSprite(transform.position.x,transform.position.y,true);
		Vector<float> dir = target->position - transform.position;

		if (dir.SqrMagnitude()<1.5)
		{
			return;
		}

		dir = dir.Normalize();
		velocity = dir * 5;

		

	}
};

class Player :public GameObject, public Rigidbody
{
public:
	Player(TileMap* map) :
		GameObject(new Sprite(LoadSprFile("./Assets/Zelda/JumpMan.spr"),tileScale)),
		Rigidbody(&transform)
	{

		useGravity = false;

		damping = 0;

		//设置地图的碰撞信息
		SetCollisionTileID({25,26,33,34,19,20,21,27,29,35,36,37});
		UpdateMap(map);
	}

	void ActorControl() {
		velocity.x = 0;
		velocity.y = 0;

		if (inputSystem.GetKeyHold(KeyA))
		{
			velocity.x = -speed;
		}

		if (inputSystem.GetKeyHold(KeyD))
		{
			velocity.x = speed;
		}

		if (inputSystem.GetKeyHold(KeyW))
		{
			velocity.y = -speed;
		}

		if (inputSystem.GetKeyHold(KeyS))
		{
			velocity.y = speed;
		}

	}

	void Start() override {

	}

	void Update() override {
		ActorControl();
	}
private:
	float speed = 8;
};

class MainMenuScene :public Scene {
public:
	MainMenuScene() :Scene("MainMenuScene") {

		text.position = Vector<int>(10, 10);
		text.textColor = WHITE;

		//button.sprite = new Sprite(LoadSprFile("./Assets/Zelda/JumpMan.spr"), Vector<int>(1, 1));
		button.text = "Start";
		button.position = Vector<int>(pixelWidth / 2, pixelHeight / 2);
		button.AddClickEvent(
			[this]()
			{
				sceneManager.LoadScene("StartScene");
			});

		image.position = Vector<int>(0, 0);
		image.scale = Vector<int>(pixelWidth, pixelHeight);
		image.color = BLACK;

		this->OnEnable();
		canvase.AddComponent(&image);
		canvase.AddComponent(&button);
		canvase.AddComponent(&text);
	}

	void OnEnable() override
	{
		canvase.SetActive(true);
	}

	void Start() override {

	}

	void Update() override 
	{
		text.text = "FPS:" + to_string(1 / deltaTime);
	}

	void OnDisable() override {
		canvase.SetActive(false);
	}

private:
	//ui
	NoaCanvase canvase;

	NoaText text;
	NoaButton button;
	NoaImage image;

};

class StartScene :public Scene {

public:
	StartScene():Scene("StartScene")
	{
		//test.target = &player.transform;
		player.transform.position = { 2,2 };
	}

	void OnEnable() override
	{
		player.SetActive(true);
		//player.isActive = true;
		player.isFrozen = false;
	}

	void Start() override 
	{
		
	}

	void Update() override {
		Vector<int> drawPos = camera.Render(tileMap, frontDelta, endDelta);
		player.sprite->DrawSprite(drawPos.x, drawPos.y, true);
		if (inputSystem.GetKeyDown(KeyESC)) 
		{
			sceneManager.LoadScene("MainMenuScene");
		}
	}

	void OnDisable() override 
	{
		player.SetActive(false);
		//player.isActive = false;
		player.isFrozen = true;
	}

private:
	TileMap tileMap = TileMap(
		{
			LoadTileFromTsd("./Assets/Zelda/map/tileSet.tsd"),
			LoadMapFromCSV("./Assets/Zelda/map/level.csv")
		}
	);
	//Test test;
	Player player = Player(&tileMap);
	TileMapCamera camera = TileMapCamera(tileScale, &player.transform);
	Vector<float> frontDelta = Vector<float>(0.0, 0.0);
	Vector<float> endDelta = Vector<float>(0.0, -0.5);

	/*NoaButton button1;*/

};


class ZeldaGame :public NoaGameEngine 
{
public:
	ZeldaGame(int width, int height, GameWindowMode windowMode, string gameName) :
		NoaGameEngine(width,height,windowMode,gameName) 
	{
		
	}

	void Start() override 
	{

	}

	void Update() override 
	{
		Scene* scene = sceneManager.GetActiveScene();
		if (scene!=nullptr)
		{
			scene->Update();
		}
	}

private:
	//MainMenuScene mainMenuScene;
	StartScene startScene;

};

int main(int argc,char * argv)
{
	ZeldaGame game(1920/2,1080/2,NoaGameEngine::WindowMode,"Zelda");
	game.Run();
	return 0;
}

#endif // Z_ZELDA