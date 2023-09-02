#include "./core/NoaEngine.h"

using namespace noa;

class Enimy :public GameObject,public Rigidbody 
{
public:
	Enimy() :
		GameObject(new Sprite(LoadSprFile("./Assets/Wolf/caco.spr"),Vector<int>(1,1)))
		,Rigidbody(&transform)
	{
		transform.position.x = 6;
		transform.position.y = 2;
	}

	~Enimy() 
	{
		GameObject::~GameObject();
	}

};

class Player :public Behaviour ,public Rigidbody
{
public:
	Player(TileMap* map) :Behaviour(), Rigidbody(&transform)
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

		//SetPosition(107, *map);
		//transform.position.x = 5;
		//transform.position.y = 5;

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
					transform.position.x = i;
					transform.position.y = j;
				}
			}
		}
	}

	void ActorControl() {
		velocity.x = 0;
		velocity.y = 0;

		if (inputSystem.GetKeyHold(KeyW))
		{
			velocity.x += sinf(transform.eulerAngle);
			velocity.y += cosf(transform.eulerAngle);
		}

		if (inputSystem.GetKeyHold(KeyS))
		{
			velocity.x += -sinf(transform.eulerAngle);
			velocity.y += -cosf(transform.eulerAngle);
		}

		if (inputSystem.GetKeyHold(KeyA))
		{
			velocity.x += -cosf(transform.eulerAngle);
			velocity.y += sinf(transform.eulerAngle);
		}

		if (inputSystem.GetKeyHold(KeyD))
		{
			velocity.x += cosf(transform.eulerAngle);
			velocity.y += -sinf(transform.eulerAngle);
		}
		
		velocity = velocity.Normalize()*speed;

	}

	void RotateControl() 
	{
		if (inputSystem.GetMouseMoveState())
		{
			Vector<float> mouseDelta = inputSystem.GetMouseMoveDelta();
			transform.eulerAngle += 0.05*mouseDelta.x*deltaTime;
		}
	}

	void Start() override {
		
	}

	void Update() override 
	{
		ActorControl();
		hp -= deltaTime*0.00001;
	}

public:
	float speed = 12;

	Uint32 maxHp = 100;
	Uint32 hp = 100;

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
		BGM.Play(true);
	}

	void Update() override {
		//renderer.DrawRect(Vector<int>(0, 0), Vector<int>(pixelWidth, pixelHeight / 2), sky);
		camera.Render(tileMap,true,sky);
		renderer.DrawString("FPS:"+to_string(1.0/deltaTime),10,10,RED,50);
		renderer.DrawString("HP:" + to_string(player.hp) + "/" + to_string(player.maxHp),10,60,RED,50);
	}

private:
	
	TileMap tileMap = TileMap(
		LoadTileFromTsd("./Assets/Wolf/Map/tileSet.tsd"),
		LoadMapFromCSV("./Assets/Wolf/Map/level.csv")
	);
	Player player = Player(&tileMap);
	FreeCamera camera = FreeCamera(&player.transform);
	Enimy enimy;

	Sprite sky = Sprite(LoadSprFile("./Assets/Wolf/sky-sun.spr"),Vector<int>(1.0,1.0));

	Audio BGM = Audio("./Assets/Wolf/Music/theme.mp3",Music);

};

int main(int argc,char * argv[])
{
	WolfGame game(1920/2, 1080/2, NoaGameEngine::WindowMode, "Wolf");
	game.Run();

	return 0;
}