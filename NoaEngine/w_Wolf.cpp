#include "./core/NoaEngine.h"

using namespace noa;

class Enimy :public GameObject {
public:
	Enimy() :GameObject(new Sprite(LoadSprFile("./Assets/Wolf/caco.spr"),Vector<int>(1,1)))
	{
		position.x = 6;
		position.y = 2;
	}

	~Enimy() 
	{
		GameObject::~GameObject();
	}

	void Start() override 
	{

	}

	void Update() override 
	{

	}


};

class Player :public Behaviour ,public Rigidbody
{
public:
	Player(TileMap* map) :Behaviour(), Rigidbody(&position)
	{
		useGravity = false;

		SetCollisionTileID({ 25,26,33,34,19,20,21,27,29,35,36,37 });
		UpdateMap(map);

		inputSystem.inputEvent += [this]() {
			this->RotateControl();
		};

	}

	~Player() {
		Behaviour::~Behaviour();
		Rigidbody::~Rigidbody();
	}

	void ActorControl() {
		velocity.x = 0;
		velocity.y = 0;

		if (inputSystem.GetKeyHold(KeyW))
		{
			velocity.x += sinf(angle);
			velocity.y += cosf(angle);
		}

		if (inputSystem.GetKeyHold(KeyS))
		{
			velocity.x += -sinf(angle);
			velocity.y += -cosf(angle);
		}

		if (inputSystem.GetKeyHold(KeyA))
		{
			velocity.x += -cosf(angle);
			velocity.y += sinf(angle);
		}

		if (inputSystem.GetKeyHold(KeyD))
		{
			velocity.x += cosf(angle);
			velocity.y += -sinf(angle);
		}
		
		velocity = velocity.Normalize()*speed;

	}

	void RotateControl() 
	{
		if (inputSystem.GetMouseMoveState())
		{
			Vector<float> mouseDelta = inputSystem.GetMouseMoveDelta();
			angle += 0.05*mouseDelta.x*deltaTime;
		}
	}

	void Start() override {
		
	}

	void Update() override 
	{
		ActorControl();
	}

public:
	float speed = 10;

};

class WolfGame :public NoaGameEngine 
{
public:
	WolfGame(int width, int height, NoaGameEngine::GameWindowMode windowMode, string gameName) :
		NoaGameEngine(width,height,windowMode,gameName) 
	{
		
	}

	void Start() override {
		Debug("Hello");
	}

	void Update() override {
		camera.Render(tileMap);
	}

private:
	
	TileMap tileMap = TileMap(
		LoadTileFromTsd("./Assets/Wolf/Map/tileSet.tsd"),
		LoadMapFromCSV("./Assets/Wolf/Map/level.csv")
	);
	Player player = Player(&tileMap);
	FreeCamera camera = FreeCamera(&player);
	Enimy enimy;
};

int main()
{
	WolfGame game(1920 / 2, 1080 / 2, NoaGameEngine::WindowMode, "Wolf");
	game.Run();

	return 0;
}