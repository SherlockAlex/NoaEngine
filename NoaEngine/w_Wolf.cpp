#include "./core/NoaEngine.h"

using namespace noa;

namespace noa {
	void DestroyRigidbody(Rigidbody* rigid);
}

class Enimy :public GameObject,public Rigidbody 
{
public:
	Transform* player = nullptr;

public:
	void TakeDamage(int damage)
	{
		Debug("take damage:" + to_string(damage));
		hp -= damage;
		if (hp<=0)
		{
			Debug("is die");
			Destroy();
			//DestroyRigidbody(this);
		}
		
	}

	Enimy() :
		GameObject(new Sprite(LoadSprFile("./Assets/Wolf/caco.spr"),Vector<int>(1,1)))
		,Rigidbody(&transform)
	{
		transform.position.x = 6;
		transform.position.y = 6;

		gameObject = this;
	}

	~Enimy() 
	{
		Rigidbody::~Rigidbody();
		GameObject::~GameObject();
	}


	void Update() override 
	{
		//获取玩家的transform
		Vector<float> dir = player->position - transform.position;
		if (dir.SqrMagnitude()<25.0) 
		{
			return;
		}
		dir = dir.Normalize();
		velocity = dir * 5;
	}

private:
	int hp = 100;


};

class Player :public Behaviour ,public Rigidbody
{
public:
	FreeCamera* camera;

	Player(TileMap* map) :Behaviour(), Rigidbody(&transform)
	{
		useGravity = false;

		vector<int> collisionTileID;
		collisionTileID.push_back(36);
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

		if (inputSystem.GetKeyHold(KeyESC)) 
		{
			inputSystem.SetRelativeMouseMode(false);
		}

		if (inputSystem.GetMouseButton(LeftButton)) 
		{
			
			//shotAFX.Play(false);
			gunShot->Play();
			
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

	void Start() override 
	{
		//gunNormal->LoadFromAnimatorFile("./Assets/Wolf/gun-normal.amt");
		gunShot->LoadFromAnimatorFile("./Assets/Wolf/lgun-shot.amt");
		gunShot->SetFrameEvent(2, [this]() {
			shotAFX.Play(false);

			Enimy * enimy = camera->GetRayHitInfoAs<Enimy*>(pixelWidth * 0.5);
			if (enimy!=nullptr)
			{
				enimy->TakeDamage(20);
			}


			Debug("shot");
			});
	}

	void Update() override 
	{
		ActorControl();
		hp -= deltaTime*0.00001;
		gunSprite.UpdateImage(gunShot->GetCurrentFrameImage());
		gunSprite.DrawSprite(0.25*pixelWidth,pixelHeight - 0.5*pixelWidth,true);
	}

public:
	float speed = 12;

	Uint32 maxHp = 100;
	Uint32 hp = 100;

	Sprite gunSprite = Sprite(
		LoadSprFile("./Assets/Wolf/gun.spr")
		,Vector<int>(0.5*pixelWidth, 0.5 * pixelWidth));

	Audio shotAFX = Audio("./Assets/Wolf/Music/shotgun.wav",Chunk);

	Sprite bulletSprite = Sprite(
		LoadSprFile("./Assets/Wolf/bullet.spr"),
		Vector<int>(0.1*pixelWidth,0.1*pixelWidth)
	);
	//bool isShot = false;

	Animator* gunShot = new Animator(14);

};

class WolfGame :public NoaGameEngine 
{
public:
	WolfGame(int width, int height, NoaGameEngine::GameWindowMode windowMode, string gameName) :
		NoaGameEngine(width,height,windowMode,gameName) 
	{
		enimy.player = & player.transform;
		player.camera = &camera;
	}

	void Start() override 
	{
		BGM.Play(true);
		inputSystem.SetRelativeMouseMode(true);
	}

	void Update() override {
		camera.Render(tileMap,true,sky);
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
	WolfGame game(1920/4, 1080/4, NoaGameEngine::WindowMode, "Wolf");
	game.Run();

	return 0;
}