#include "./core/NoaEngine.h"

using namespace noa;

class Enimy :public GameObject,public Rigidbody 
{
public:
	Transform* player = nullptr;

public:
	void TakeDamage(int damage)
	{
		Debug("take "+tag+" damage:" + to_string(damage));
		hp -= damage;
		if (hp<=0)
		{
			
			isFrozen = true;
			die.Play();
			Debug("is die");
			//Destroy();
			//DestroyRigidbody(this);
		}
		
	}

	Enimy(Sprite sprite) :
		GameObject(new Sprite(sprite))
		,Rigidbody(&transform)
	{

		tag = "Enimy";

		die.LoadFromAnimatorFile("./Assets/Wolf/caco-die.amt");
		die.SetFrameEvent(10, [this]() 
			{
				
				Destroy();
				RemoveRigidbody();
			});

		gameObject = this;
	}

	~Enimy() 
	{
		Rigidbody::~Rigidbody();
		GameObject::~GameObject();
	}


	void Update() override 
	{
		sprite->UpdateImage(die.GetCurrentFrameImage());

		////获取玩家的transform
		//Vector<float> dir = player->position - transform.position - Vector<float>(0.5, 0.5);
		//if (dir.SqrMagnitude()<25.0) 
		//{
		//	return;
		//}
		//dir = dir.Normalize();
		//velocity = dir * 5;
	}

private:
	int hp = 100;
	Animator die = Animator(5);

};

class Item :public GameObject, public Rigidbody {
public:

	NoaEvent<void()> pickEvent;

	Item(Sprite* sprite) :GameObject(sprite), Rigidbody(&transform) {
		
		useGravity = false;
		collision.isTrigger = true;

		transform.position.x = 2;
		transform.position.y = 2;

		tag = "item";

	}

	void OnTrigger(void * other) override
	{
		if (other == nullptr)
		{
			return;
		}
		Rigidbody* rigid = (Rigidbody*)other;
		if (rigid->tag == "Player")
		{
			RemoveRigidbody();
			Destroy();
			Debug("Pick up the bullet");
		}
	}

	void OnDisable() override
	{
		pickEvent.Invoke();
	}

};

class Player :public Behaviour ,public Rigidbody
{
public:
	FreeCamera* camera;

	Player(TileMap* map) :Behaviour(), Rigidbody(&transform)
	{
		tag = "Player";

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

		//SetPosition(107, *map);

	}

	~Player() {
		Behaviour::~Behaviour();
		Rigidbody::~Rigidbody();
	}

	void SetPosition(int tileID,MapFile & tileMap)
	{
		for (int i=0;i<tileMap.w;i++) 
		{
			for (int j=0;j<tileMap.h;j++) 
			{
				if (tileMap.image[j*tileMap.w + i] == tileID)
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
			if (bulletCount>0)
			{
				gunShot->Play();
			}
			
			
		}
		
		velocity = velocity.Normalize() * speed;
		

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
		gunShot->SetFrameEvent(2, [this]() 
			{
			bulletCount--;
			shotAFX.Play(false);

			Enimy * enimy = nullptr;

			for (int i = 0.5*pixelWidth-0.01*pixelWidth;i<=0.5*pixelWidth + 0.01 * pixelWidth;i++)
			{
				enimy = camera->GetRayHitInfoAs<Enimy*>(i);
				
				if (enimy!=nullptr)
				{
					break;
				}
			}

			if (enimy!=nullptr)
			{
				if (enimy->tag != "Enimy")
				{
					return;
				}
				enimy->TakeDamage(20);
			}


			//Debug("shot");
			});

	}

	void Update() override 
	{
		ActorControl();
		gunSprite.UpdateImage(gunShot->GetCurrentFrameImage());
		gunSprite.DrawSprite(0.25*pixelWidth,pixelHeight - 0.5*pixelWidth,true);
		renderer.DrawString("hp:100\nbullet:" + to_string(bulletCount), 0, 0, RED, pixelHeight / 20);
	}

public:
	float speed = 7;

	Uint32 maxHp = 100;
	Uint32 hp = 100;

	Sprite gunSprite = Sprite(
		LoadSprFile("./Assets/Wolf/gun.spr")
		,Vector<int>(0.5*pixelWidth, 0.5 * pixelWidth));

	Audio shotAFX = Audio("./Assets/Wolf/Music/handleGunShot.mp3",Chunk);

	int bulletCount = 100;

	Animator* gunShot = new Animator(14);

};

class WolfGame :public NoaGameEngine 
{
public:
	WolfGame(int width, int height, NoaGameEngine::GameWindowMode windowMode, string gameName) :
		NoaGameEngine(width,height,windowMode,gameName) 
	{
		player.SetPosition(35, objectMap);
		//bulletAnimator.LoadFromAnimatorFile("./Assets/Wolf/bullet.amt");
		//bulletSprite = new Sprite(bulletAnimator.GetCurrentFrameImage(),Vector<int>(64,64));
		

		for (int i = 0;i<objectMap.w;i++) 
		{
			for (int j = 0;j<objectMap.h;j++) 
			{
				if (objectMap.image[j*objectMap.w+i] == 18)
				{
					Enimy* enimy = new Enimy(cacoSprite);
					enimy->player = &player.transform;
					enimy->transform.position.x = i;
					enimy->transform.position.y = j;
				}
			}
		}

		for (int i = 0; i < objectMap.w; i++)
		{
			for (int j = 0; j < objectMap.h; j++)
			{
				if (objectMap.image[j * objectMap.w + i] == 28)
				{
					Item* bullet = new Item(&bulletSprite);
					bullet->transform.position.x = i;
					bullet->transform.position.y = j;

					bullet->pickEvent += [this]() {
						player.bulletCount += 5;
						bulletPickUpSFX.Play(false);
						};
				}
			}
		}

		player.camera = &camera;

		

	}

	void Start() override 
	{
		//BGM.Play(true);
		inputSystem.SetRelativeMouseMode(true);
	}

	void Update() override {
		camera.Render(tileMap,true,sky);
		mouse.DrawSprite(pixelWidth*0.5-0.5*mouse.scale.x,pixelHeight*0.5-0.5*mouse.scale.y,true);
		if (inputSystem.GetKeyHold(KeyM))
		{
			mapCamera.Render(tileMap,Vector<float>(0.0,0.0),Vector<float>(0.0,0.0));
		}
	}

private:
	
	Sprite sky = Sprite(LoadSprFile("./Assets/Wolf/sky-sun.spr"), Vector<int>(1.0, 1.0));
	TileMap tileMap = TileMap(
		LoadTileFromTsd("./Assets/Wolf/Map/tileSet.tsd"),
		LoadMapFromCSV("./Assets/Wolf/Map/level_地图层.csv")
	);

	Player player = Player(&tileMap);
	FreeCamera camera = FreeCamera(&player.transform);
	TileMapCamera mapCamera = TileMapCamera(Vector<int>(32,32),&player.transform);
	Sprite mouse = Sprite(LoadSprFile("./Assets/Wolf/mouse.spr"),Vector<int>(0.03*pixelWidth, 0.03 * pixelWidth));
	
	Sprite cacoSprite = Sprite(LoadSprFile("./Assets/Wolf/caco.spr"),Vector<int>(1.0,1.0));

	//Animator bulletAnimator = Animator(7);
	Sprite bulletSprite = Sprite(LoadSprFile("./Assets/Wolf/bullet.spr"),Vector<int>(64,64));

	Audio bulletPickUpSFX = Audio("./Assets/Wolf/Music/pickUpBullet.mp3",Chunk);

	Audio BGM = Audio("./Assets/Wolf/Music/theme.mp3",Music);

	MapFile objectMap = LoadMapFromCSV("./Assets/Wolf/Map/level_对象.csv");

};

int main(int argc,char * argv[])
{
	WolfGame game(1920/2, 1080/2, NoaGameEngine::WindowMode, "Wolf");
	game.Run();

	return 0;
}