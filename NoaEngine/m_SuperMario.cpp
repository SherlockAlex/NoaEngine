#define M_SUPERMARIO
#ifdef M_SUPERMARIO
#include "./core/NoaEngine.h"

using namespace noa;

TileMap* currentMap = nullptr;
Vector<int> tileScale = Vector<int>(70, 70);

class Player:public GameObject,public Rigidbody
{
public:
	Player() :GameObject(new Sprite(resource.LoadSprFile("./Assets/JumpMan/JumpMan.spr"), tileScale)), Rigidbody(&transform)
	{
		transform.position = Vector<float>(0.0, 0.0);
		//useGravity = false;
		damping = 0;
		idle->LoadFromAnimationFile("./Assets/JumpMan/Animator/mario_idle.amt");
		run->LoadFromAnimationFile("./Assets/JumpMan/Animator/mario_run.amt");
		jump->LoadFromAnimationFile("./Assets/JumpMan/Animator/mario_jump.amt");

		currentAnimatorState = idle;

	}
	~Player()
	{
		GameObject::~GameObject();
		Rigidbody::~Rigidbody();
	}

	void InitPosition(TileMap& tileMap,const int targetTileID) {
		for (int i = 0; i < tileMap.w; i++)
		{
			for (int j = 0; j < tileMap.h; j++)
			{

				if (tileMap.GetTileID(i, j) == targetTileID)
				{
					transform.position.x = i;
					transform.position.y = j;
					velocity.y = 0;
				}

			}
		}
	}

	void ActorControl() {
		velocity.x = 0;
		//velocity.y = 0;

		if (inputSystem.GetKeyHold(KeyA))
		{
			isLeft = true;
			velocity.x = -speed;
		}
		if (inputSystem.GetKeyHold(KeyD))
		{
			isLeft = false;
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

		//Debug(to_string(collision.isGrounded));
		if ((inputSystem.GetKeyHold(KeySpace)||inputSystem.GetKeyHold(KeyK)) && collision.isGrounded)
		{
			AddForce(jumpForce, Impulse);
			jumpSFX.Play(false);
		}
	}

	void AnimatorControl() {
		if (NoaAbs<float>(velocity.y * deltaTime)<0.001) {
			currentAnimatorState = idle;
		}
		else {
			currentAnimatorState = jump;
		}

		if (inputSystem.GetKeyHold(KeyA)|| inputSystem.GetKeyHold(KeyD))
		{
			//isLeft = true;

			if (NoaAbs<float>(velocity.y * deltaTime) < 0.001)
			{
				currentAnimatorState = run;
			}
			else {
				currentAnimatorState = jump;
			}

		}

		currentAnimatorState->Play();

		const SpriteFile nextFrame = move(currentAnimatorState->GetCurrentFrameImage());
		sprite->UpdateImage(nextFrame);

	}

	void Start() override 
	{
		gravityWeight = 3.5;
	}

	Vector<float> jumpForce = Vector<float>(0.0, -15);
	void Update() override 
	{

		ActorControl();
		AnimatorControl();

		if (currentMap->GetTileID(transform.position.x+0.5,transform.position.y+0.5)==0)
		{
			currentMap->level[int(transform.position.y+0.5) * currentMap->w + int(transform.position.x+0.5)] = 5;
			coinSFX.Play(false);
		}

	}

public:
	float speed = 8;
	bool isLeft = false;

	const Audio coinSFX = Audio("./Assets/JumpMan/Music/coin.mp3", Chunk);
	const Audio jumpSFX = Audio("./Assets/JumpMan/Music/jump.mp3", Chunk);

	Animation* currentAnimatorState = nullptr;
	Animation* idle = new Animation(5,false);
	Animation* run = new Animation(10,false);
	Animation* jump = new Animation(10,false);

};

class Platformer :public NoaGameEngine {
public:
	Platformer(int width, int height, GameWindowMode windowMode, string gameName) :NoaGameEngine(width, height, windowMode, gameName) 
	{
		player.UpdateMap(&tileMap);
		tileMap.SetCollisionTileID({ 1,2 });
		player.InitPosition(tileMap, 87);

		BGM.Play(true);

		currentMap = &tileMap;

		
	}

	void Start() override
	{

	}

	void Update() override 
	{

		Vector<int> playerDrawPos = camera.Render(tileMap,frontDelta,endDelta);

		//Draw player
		player.sprite->DrawSprite(playerDrawPos.x, playerDrawPos.y, true, !player.isLeft);


	}

private:
	//��ͼ
	//0 - wall
	//1 - coin
	TileMap tileMap = TileMap(
		resource.LoadTileFromTsd("./Assets/JumpMan/Tile/tileSet.tsd"),
		resource.LoadMapFromCSV("./Assets/JumpMan/Map/level1.csv")
	);
	
	//���
	Player player;
	
	//���
	TileMapCamera camera = TileMapCamera(tileScale,&player.transform);
	Vector<float> frontDelta = Vector<float>(0.0,0.0);
	Vector<float> endDelta = Vector<float>(-1, -1);

	//��Ч
	Audio BGM = Audio("./Assets/JumpMan/Music/BGM.ogg", Music);
	Audio gameOverMusic = Audio("./Assets/JumpMan/Music/gameover.mp3", Chunk);

};

int main(int argc,char * argv[])
{
	Platformer game(1920 / 2, 1080 / 2, NoaGameEngine::WindowMode, "SuperMario");
	game.Run();
	return 0;
}
#endif