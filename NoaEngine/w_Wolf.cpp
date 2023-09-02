#include "./core/NoaEngine.h"

using namespace noa;

class Enimy :public GameObject,public Rigidbody 
{
public:
	Enimy() :
		GameObject(new Sprite(LoadSprFile("./Assets/Wolf/caco.spr"),Vector<int>(1,1)))
		,Rigidbody(&position)
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

		//SetCollisionTileID({ 25,26,33,34,19,20,21,27,29,35,36,37 });
		vector<int> collisionTileID;
		for (int i=0;i<108;i++)
		{
			if (i == 107)
			{
				continue;
			}

			collisionTileID.push_back(i);

		}
		SetCollisionTileID(collisionTileID);
		UpdateMap(map);

		inputSystem.inputEvent += [this]() {
			this->RotateControl();
		};

		SetPosition(107, *map);
		

	}

	~Player() {
		Behaviour::~Behaviour();
		Rigidbody::~Rigidbody();
	}

	void SetPosition(int tileID,TileMap & tileMap)
	{
		for (int i=0;i<tileMap.w;i++) 
		{
			for (int j=0;j<tileMap.h;j++) 
			{
				if (tileMap.GetTileID(i,j)==tileID)
				{
					position.x = i;
					position.y = j;
				}
			}
		}
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
	float speed = 12;

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
		camera.Render(tileMap,107);
		renderer.DrawString("FPS:"+to_string(1.0/deltaTime),10,10,RED,50);
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
	WolfGame game(1920/2, 1080/2, NoaGameEngine::WindowMode, "Wolf");
	game.Run();

	return 0;
}