#define MARIO_GAME
#ifdef MARIO_GAME

#include "NoaEngine.h"

/// <summary>
/// 类似《超级马里奥》的游戏
/// </summary>

using namespace noa;

extern vector<GameObject*> gameObjects;

TileMap* currentMap;

int coinCount = 0;
bool gameOver = false;

static Audio * BGM;
static Audio * gameOverMusic;

class Player :public GameObject,public Rigidbody
{
private:
	function<void()> playerControlCallback;
	

public:
	const float speed = 10;
	bool isJump = false;
	float jumpTime = 0.0;
	float jumpSpeed = 0;
	bool isLeft = false;
	
	const Audio coinSFX = Audio("./Assets/JumpMan/Music/coin.mp3", Chunk);
	const Audio jumpSFX = Audio("./Assets/JumpMan/Music/jump.mp3", Chunk);

	Animator* currentAnimatorState = nullptr;
	Animator* idle = new Animator(5);
	Animator* run = new Animator(10);
	Animator* jump = new Animator(10);

public:
	Player():GameObject(new Sprite(LoadSprFile("./Assets/JumpMan/JumpMan.spr"), 11)),Rigidbody(&(this->position))
	{
		//sprite = new Sprite(LoadSprFile("./Assets/JumpMan/JumpMan.spr"), 11);

		//玩家的构造函数
		colliderSize.x = 0.45;
		colliderSize.y = 0.55;

		idle->LoadFromAnimatorFile("./Assets/JumpMan/Animator/mario_idle.amt");
		run->LoadFromAnimatorFile("./Assets/JumpMan/Animator/mario_run.amt");
		jump->LoadFromAnimatorFile("./Assets/JumpMan/Animator/mario_jump.amt");

		currentAnimatorState = idle;
	}

	~Player() {
		delete idle;
		delete run;
		delete jump;
	}

public:

	void InputEventTest() 
	{
		Debug("the input event is invoke");
	}

	static Vector<float> pos;
	Vector<float> jumpImpuls=Vector<float>(0, -15);

	void Control() 
	{
		if (isGrounded) {
			currentAnimatorState = idle;
			if (velocity.y == 0) 
			{
				isJump = false;
			}
		}
		else {
			currentAnimatorState = jump;
		}
		

		if (inputSystem.GetKeyHold(KeyA))
		{
			//AddForce(Vector<float>(-1, 0), ContinuousForce);
			isLeft = true;
			position.x -= deltaTime * speed;
			
			/*pos.x = position.x - colliderSize.x;
			pos.y = position.y + colliderSize.y;
			if (ColliderWithMap(0, pos))
			{
				position.x += deltaTime * speed;
			}*/

			if (isGrounded) 
			{
				currentAnimatorState = run;
			}
			else {
				currentAnimatorState = jump;
			}

		}

		if (inputSystem.GetKeyHold(KeyD))
		{
			//AddForce(Vector<float>(1, 0), ContinuousForce);
			//Debug("Move right");
			isLeft = false;
			position.x += deltaTime * speed;
			/*pos.x = position.x + colliderSize.x;
			pos.y = position.y + colliderSize.y;
			if (ColliderWithMap(0, pos))
			{
				position.x -= deltaTime * speed;
			}*/
			
			if (isGrounded)
			{
				currentAnimatorState = run;
			}
			else {
				currentAnimatorState = jump;
			}

		}

		if (inputSystem.GetKeyHold(KeyK)&&isGrounded)
		{
			if (!isJump)
			{
				Debug("Jump");
				jumpSFX.Play(false);
				currentAnimatorState = jump;
				AddForce(jumpImpuls, Impulse);
				isJump = true;
			}
			
		}

		////下落的碰撞检测

		//pos.x = position.x;
		//pos.y = position.y + colliderSize.y;
		//if (isGrounded&&ColliderWithMap(0, pos))
		//{
		//	position.y -= 0.05;
		//}
		//pos.x = position.x - colliderSize.x;
		//pos.y = position.y + colliderSize.y;
		//if (isGrounded && ColliderWithMap(0, pos))
		//{
		//	position.y -= 0.05;
		//}
		//pos.x = position.x + colliderSize.x;
		//pos.y = position.y + colliderSize.y;
		//if (isGrounded && ColliderWithMap(0, pos))
		//{
		//	position.y -= 0.05;
		//}

		////检测头顶是否顶砖块
		//pos.x = position.x;
		//pos.y = position.y - colliderSize.y;
		//if (ColliderWithMap(0, pos))
		//{
		//	position.y += velocity.y * deltaTime;
		//	velocity.y = 5;
		//}
		//pos.x = position.x - colliderSize.x;
		//pos.y = position.y - colliderSize.y;
		//if (ColliderWithMap(0, pos))
		//{
		//	position.y += velocity.y * deltaTime;
		//	velocity.y = 5;
		//}
		//pos.x = position.x + colliderSize.x;
		//pos.y = position.y - colliderSize.y;
		//if (ColliderWithMap(0, pos))
		//{
		//	position.y += velocity.y * deltaTime;
		//	velocity.y = 5;
		//}

	}

	void Start() override 
	{

	}

	Vector<int> testPos;
	void Update() override 
	{
		Control();

		testPos.x = position.x;
		testPos.y = position.y;

		if (currentMap==nullptr)
		{
			velocity.y = 0;
			velocity.x = 0;
			return;
		}

		const int hitItemIndex = testPos.y * currentMap->w + testPos.x;
		const Uint8 hitItem = currentMap->level[hitItemIndex];
		if (hitItem == 1) 
		{
			coinCount++;
			//将金币瓦片动态更新为其他类型瓦片
			currentMap->level[testPos.y * currentMap->w + testPos.x] = 25;
			coinSFX.Play(false);
			Debug("coin count:" + to_string(coinCount));
		}

		if (hitItem == 37) 
		{

			gameOver = true;
			position.x = 0;
			position.y = 0;

			BGM->Stop();
			gameOverMusic->Play(false);

		}

		currentAnimatorState->Play();

		const SpriteFile nextFrame = move(currentAnimatorState->GetCurrentFrameImage());
		sprite->UpdateImage(nextFrame);

		isGrounded = false;

		const int groundIndex = (int)(position.y + colliderSize.y) * currentMap->w + (int)position.x;
		const int leftGroundIndex = (int)(position.y + colliderSize.y) * currentMap->w + (int)(position.x - colliderSize.x);
		const int rightGroundIndex = (int)(position.y + colliderSize.y) * currentMap->w + (int)(position.x + colliderSize.x);
		
		if (currentMap->level[groundIndex] == 0 || currentMap->level[leftGroundIndex] == 0 || currentMap->level[rightGroundIndex] == 0)
		{
			isGrounded = true;
		}

		// 减少函数调用次数
		const int newPositionY = (int)(position.y + colliderSize.y);
		const int newPositionX = (int)position.x;

		if (currentMap->level[newPositionY * currentMap->w + newPositionX] == 0 ||
			currentMap->level[newPositionY * currentMap->w + (int)(position.x - colliderSize.x)] == 0 ||
			currentMap->level[newPositionY * currentMap->w + (int)(position.x + colliderSize.x)] == 0)
		{
			isGrounded = true;
		}
		

	}

};

class Mario :public NoaGameEngine
{
private:
	float deltaSize;
	Player* player;

	Sprite gameOverTexture;
	Sprite skyboxTexture;

public:
	Mario(int width, int height,
		GameWindowMode windowMode,
		string gameName) :NoaGameEngine(width, height,
			windowMode,
			gameName)
	{
		player = new Player();
		gameOverTexture = Sprite(LoadSprFile("./Assets/JumpMan/Texture/gameover.spr"), 1);
		skyboxTexture = Sprite(LoadSprFile("./Assets/JumpMan/Texture/skybox.spr"), 1);
		BGM = new Audio("./Assets/JumpMan/Music/BGM.ogg", Music);
		gameOverMusic = new Audio("./Assets/JumpMan/Music/gameover.mp3", Chunk);

		currentMap = new TileMap(LoadTileFromTsd("./Assets/JumpMan/Tile/tileSet.tsd"), LoadMapFromCSV("./Assets/JumpMan/Map/level1.csv"));


		deltaSize = (float)currentMap->h / (float)pixelHeight;
		//deltaSize 最后的系数
		deltaSize *= 0.3;

		for (int i = 0;i < currentMap->w;i++) 
		{
			for (int j = 0; j < currentMap->h;j++)
			{

				if (currentMap ->level[j*currentMap->w+i] == 61)
				{
					player->position.x = i;
					player->position.y = j;
					player->velocity.y = 0;
				}

			}
		}

	}

	~Mario() {
		NoaGameEngine::~NoaGameEngine();
		delete player;
		delete currentMap;
		delete BGM;
		delete gameOverMusic;
	}

public:
	Vector<int> pixelPos;
	Vector<float> simple;
	void Draw() 
	{

		//对于多图层的渲染
		//一般的思路是将所有的图像的像素进行叠加
		// 如果最顶层的像素alpha通道不为0，就直接替代这个位置的底层像素
		// 否则直接跳过
		
		const int mapLayerCount = currentMap->levelLayer.size();

		//for (int layerIndex = 0; layerIndex < mapLayerCount;layerIndex++)
		{
			for (int x = 0; x < pixelWidth; x++)
			{
				for (int y = 0; y < pixelHeight; y++)
				{

					pixelPos.x = (player->position.x - 0.5 * pixelWidth * deltaSize + x * deltaSize);
					pixelPos.y = (player->position.y - 0.5 * pixelHeight * deltaSize + y * deltaSize);

					//开始计算
					Uint8 hitByte = 10;
					if (pixelPos.y <= currentMap->h && pixelPos.y >= 0 && pixelPos.x <= currentMap->w && pixelPos.x >= 0)
					{
						hitByte = currentMap->level[
							pixelPos.y * currentMap->w +
								pixelPos.x
						];
					}

					const float testX = (player->position.x - 0.5 * pixelWidth * deltaSize + x * deltaSize);
					const float testY = (player->position.y - 0.5 * pixelHeight * deltaSize + y * deltaSize);

					simple.x = testX - (int)(testX);
					simple.y = testY - (int)(testY);

					Uint32 color = 0;

					if (currentMap->IsTile(hitByte))
					{
						color = currentMap->GetTile(hitByte).sprite->GetTransposeColor(simple.y, simple.x);
					}

					if (color == BLACK)
					{
						//渲染天空
						simple.x = x / (float)pixelWidth;
						simple.y = y / (float)pixelHeight;
						color = currentMap->GetTile(25).sprite->GetTransposeColor(simple.y, simple.x);
					}

					renderer.DrawPixel(x, y, color);
				}
			}
		}

		//绘制玩家
		player->sprite->DrawSprite(pixelWidth/2,pixelHeight/2,true,!player->isLeft);
		//绘制碰撞体的网格线
		//Debug("player position y:" + to_string(player->position.y)+",fix position y:"+to_string(player->position.y - player->colliderSize.y));
	}

	void Start() override
	{
		BGM->Play(true);
	}

	//bool canDraw = true;

	void Update() override
	{
		
		if (gameOver)
		{

			gameOverTexture.DrawSpriteFull();

			//加载地图
			delete currentMap;

			currentMap = nullptr;

			if (inputSystem.GetKeyDown(KeyK))
			{

				currentMap = new TileMap(LoadTileFromTsd("./Assets/JumpMan/Tile/tileSet.tsd"), LoadMapFromCSV("./Assets/JumpMan/Map/level1.csv"));

				for (int i = 0; i < currentMap->w; i++)
				{
					for (int j = 0; j < currentMap->h; j++)
					{

						if (currentMap->level[j * currentMap->w + i] == 61)
						{
							player->position.x = i;
							player->position.y = j;
						}

					}
				}

				//重新开始新游戏
				coinCount = 0;
				gameOver = false;
				BGM->Play(true);
			}
		}
		else {
			
			Draw();
		}
	}

};





int main(int argc,char * argv[]) 
{
	Mario mario(1920/2, 1080/2, NoaGameEngine::WindowMode, "Mario");
	mario.Run();
	return 0;
}

#endif