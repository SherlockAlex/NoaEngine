#include "NoaEngine.h"

using namespace noa;

TileMap* currentMap = nullptr;

class Player:public GameObject,public Rigidbody
{
public:
	Player() :GameObject(new Sprite(LoadSprFile("./Assets/JumpMan/JumpMan.spr"), Vector<float>(0.3,0.3))), Rigidbody(&position)
	{
		//��ҵĹ��캯��
		position = Vector<float>(0.0, 0.0);
		//useGravity = false;

		idle->LoadFromAnimatorFile("./Assets/JumpMan/Animator/mario_idle.amt");
		run->LoadFromAnimatorFile("./Assets/JumpMan/Animator/mario_run.amt");
		jump->LoadFromAnimatorFile("./Assets/JumpMan/Animator/mario_jump.amt");

		currentAnimatorState = idle;

	}
	~Player()
	{
		GameObject::~GameObject();
		Rigidbody::~Rigidbody();
	}

	void InitPosition(TileMap& tileMap,const int targetTileID) {
		//������ҵĳ�ʼλ��
		for (int i = 0; i < tileMap.w; i++)
		{
			for (int j = 0; j < tileMap.h; j++)
			{

				if (tileMap.GetTileID(i, j) == targetTileID)
				{
					position.x = i;
					position.y = j;
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


		if (inputSystem.GetKeyHold(KeySpace) && isGrounded)
		{
			AddForce(jumpForce, Impulse);
			jumpSFX.Play(false);
		}
	}

	void AnimatorControl() {
		if (isGrounded) {
			currentAnimatorState = idle;
		}
		else {
			currentAnimatorState = jump;
		}

		if (inputSystem.GetKeyHold(KeyA)|| inputSystem.GetKeyHold(KeyD))
		{
			//isLeft = true;

			if (isGrounded)
			{
				currentAnimatorState = run;
			}
			else {
				currentAnimatorState = jump;
			}

		}

		if (inputSystem.GetKeyHold(KeySpace) && isGrounded)
		{
			currentAnimatorState = jump;
		}

		currentAnimatorState->Play();

		const SpriteFile nextFrame = move(currentAnimatorState->GetCurrentFrameImage());
		sprite->UpdateImage(nextFrame);

	}

	void Start() override 
	{

	}
	Vector<float> jumpForce = Vector<float>(0.0, -15.0);
	void Update() override 
	{
		ActorControl();
		AnimatorControl();

		if (currentMap->GetTileID(position.x+0.5,position.y)==1)
		{
			currentMap->level[position.y * currentMap->w + position.x+0.5] = 25;
			coinSFX.Play(false);
		}

	}

public:
	float speed = 10;

	//��Ч
	const Audio coinSFX = Audio("./Assets/JumpMan/Music/coin.mp3", Chunk);
	const Audio jumpSFX = Audio("./Assets/JumpMan/Music/jump.mp3", Chunk);

	//����
	Animator* currentAnimatorState = nullptr;
	Animator* idle = new Animator(5);
	Animator* run = new Animator(10);
	Animator* jump = new Animator(10);

};

class Platformer :public NoaGameEngine {
public:
	Platformer(int width, int height, GameWindowMode windowMode, string gameName) :NoaGameEngine(width, height, windowMode, gameName) 
	{
		player.UpdateMap(&tileMap);
		player.SetCollisionTileID({ 0 });
		player.InitPosition(tileMap, 61);

		BGM.Play(true);

		currentMap = &tileMap;

		
	}

	void Start() override
	{

	}

	void Update() override 
	{

		//��������Ⱦ2d��Ϸ

		Vector<float> offset = camera.Render(tileMap);

		//Draw player
		renderer.DrawRect(
			Vector<int>((player.position.x - offset.x) * tileScale.x, (player.position.y - offset.y) * tileScale.y),
			Vector<int>((player.position.x - offset.x+1) * tileScale.x, (player.position.y - offset.y+1) * tileScale.y),
			*player.sprite);


	}

private:
	//��ͼ
	//0 - wall
	//1 - coin
	TileMap tileMap = TileMap(
		LoadTileFromTsd("./Assets/JumpMan/Tile/tileSet.tsd"),
		LoadMapFromCSV("./Assets/JumpMan/Map/level1.csv")
	);
	
	//���
	Player player;
	
	//���
	Vector<int> tileScale = Vector<int>(64, 64);
	TileMapCamera camera = TileMapCamera(tileScale,&player.position);

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