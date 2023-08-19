#include "NoaEngine.h"

/// <summary>
/// 类似《超级马里奥》的游戏
/// </summary>

extern vector<GameObject*> gameObjects;

LevelMap* currentMap;

int coinCount = 0;
bool gameOver = false;

static bool ColliderWithMap(Uint8 byte, Vector<float> colliderPos);

static Audio * BGM;
static Audio * gameOverMusic;

class Cloud:public GameObject
{
public:
	Cloud(float x,float y,Sprite sprite) :GameObject(sprite) 
	{
		position = Vector<float>(x, y);
	}

public:
	void Start() override {

	}
	void Update() override {

	}

	

};

class Player :public GameObject
{
private:
	function<void()> playerControlCallback;

public:
	float speed = 10;
	bool isJump = false;
	float jumpTime = 0.0;
	float jumpSpeed = 0;
	bool isLeft = false;
	bool isGrounded = false;

	Vector<float> colliderSize;
	Audio coinSFX = Audio("./Assets/JumpMan/Music/coin.mp3", Chunk);
	Audio jumpSFX = Audio("./Assets/JumpMan/Music/jump.mp3", Chunk);

	//Animator
	Animator* currentAnimatorState = nullptr;

	Animator* idle = new Animator(10);
	Animator* run = new Animator(10);
	Animator* jump = new Animator(10);

public:
	Player(Sprite sprite):GameObject(sprite)
	{
		//玩家的构造函数
		colliderSize.x = 0.6;
		colliderSize.y = 0.7;

		idle->LoadFromAnimatorFile("./Assets/JumpMan/Animator/mario_idle.amt");
		run->LoadFromAnimatorFile("./Assets/JumpMan/Animator/mario_run.amt");
		jump->LoadFromAnimatorFile("./Assets/JumpMan/Animator/mario_jump.amt");

		currentAnimatorState = idle;
	}

public:

	void Control() 
	{
		if (isGrounded) {
			currentAnimatorState = idle;
		}
		else {
			currentAnimatorState = jump;
		}
		

		if (inputSystem.GetKeyHold(KeyA))
		{
			isLeft = true;
			position.x -= deltaTime * speed;
			Vector<float> pos;
			pos.x = position.x - colliderSize.x;
			pos.y = position.y;
			if (ColliderWithMap(255, pos))
			{
				position.x += deltaTime * speed;
			}

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
			//Debug("Move right");
			isLeft = false;
			position.x += deltaTime * speed;
			Vector<float> pos;
			pos.x = position.x + colliderSize.x;
			pos.y = position.y;
			if (ColliderWithMap(255, pos))
			{
				position.x -= deltaTime * speed;
			}
			
			if (isGrounded)
			{
				currentAnimatorState = run;
			}
			else {
				currentAnimatorState = jump;
			}

		}

		if (inputSystem.GetKeyDown(KeyK))
		{
			if (isGrounded)
			{
				//跳跃
				if (!isJump)
				{
					isJump = true;
					jumpSFX.Play(false);
					currentAnimatorState = jump;
				}
			}
			
		}

		
		if (isJump)
		{
			if (jumpTime < 0.35)
			{
				jumpTime += deltaTime;
				jumpSpeed -= 10 * deltaTime;

				currentAnimatorState = jump;

				position.y -= 10 * 0.8 * deltaTime;
				Vector<float> pos;
				pos.x = position.x;
				pos.y = position.y - colliderSize.y;
				if (ColliderWithMap(255, pos))
				{
					position.y += 10 * 0.8 * deltaTime;
					jumpTime = 0.0;
					isJump = false;
					Debug("jump over");
				}
				pos.x = position.x - colliderSize.x;
				pos.y = position.y - colliderSize.y;
				if (ColliderWithMap(255, pos))
				{
					position.y += 10 * 0.8 * deltaTime;
					jumpTime = 0.0;
					isJump = false;
					Debug("jump over");
				}
				pos.x = position.x + colliderSize.x;
				pos.y = position.y - colliderSize.y;
				if (ColliderWithMap(255, pos))
				{
					position.y += 10 * 0.8 * deltaTime;
					jumpTime = 0.0;
					isJump = false;
					Debug("jump over");
				}

				if (jumpTime >= 0.35)
				{
					jumpTime = 0.0;
					isJump = false;
					Debug("jump over");
				}

			}

		}
		else {
			position.y += 10 * 1.2 * deltaTime;
			Vector<float> pos;
			pos.x = position.x;
			pos.y = position.y + colliderSize.y;
			if (ColliderWithMap(255, pos))
			{
				position.y -= 10 * 1.2 * deltaTime;
			}
			pos.x = position.x - colliderSize.x;
			pos.y = position.y + colliderSize.y;
			if (ColliderWithMap(255, pos))
			{
				position.y -= 10 * 1.2 * deltaTime;
			}
			pos.x = position.x + colliderSize.x;
			pos.y = position.y + colliderSize.y;
			if (ColliderWithMap(255, pos))
			{
				position.y -= 10 * 1.2 * deltaTime;
			}
			
			

		}

	}

	void Start() override 
	{
		
#ifdef __linux
		playerControlCallback = [this]() {
			this->Control();
		};
		inputSystem.BindEvent(playerControlCallback);
#endif // __linux
	}

	void Update() override 
	{
		//float oldy = position.y;

#ifdef _WIN64
		Control();
#endif // _WIN64
		Vector<int> testPos(position.x,position.y);
		Uint8 hitItem = currentMap->level[testPos.y * currentMap->w + testPos.x];
		if (hitItem == 127) 
		{
			//Debug("Get a Coin");
			coinCount++;
			currentMap->level[testPos.y * currentMap->w + testPos.x] = 0;
			coinSFX.Play(false);
			Debug("coin count:" + to_string(coinCount));
			//cout << "coin count:" << coinCount << endl;
		}

		if (hitItem == 200) 
		{

			gameOver = true;
			position.x = 0;
			position.y = 0;

			BGM->Stop();
			gameOverMusic->Play(false);

		}

		currentAnimatorState->Play();
		sprite.UpdateImage(*(currentAnimatorState->GetCurrentFrameImage()));

		//isGrounded = (oldy == position.y);
		isGrounded = (currentMap->level[
			(int)(position.y+ colliderSize.y+10*1.2*deltaTime) * currentMap->w + (int)position.x] 
			== 255)|| 
			(currentMap->level[
				(int)(position.y + colliderSize.y + 10 * 1.2 * deltaTime) * currentMap->w + (int)(position.x-colliderSize.x)]
				== 255) || (currentMap->level[
					(int)(position.y + colliderSize.y + 10 * 1.2 * deltaTime) * currentMap->w + (int)(position.x + colliderSize.x)]
					== 255);

	}

};

class Mario :public NoaGameEngine
{
private:
	float deltaSize;
	Player* player;

	Sprite wallTexture;
	Sprite coinTexture;
	Sprite gameOverTexture;

	//Audio BGM = Audio("./Assets/JumpMan/Music/BGM.ogg", Music);
	//Audio gameOverMusic = Audio("./Assets/JumpMan/Music/gameover.mp3", Chunk);


public:
	Mario(int width, int height,
		GameWindowMode windowMode,
		string gameName) :NoaGameEngine(width, height,
			windowMode,
			gameName)
	{
		player = new Player(Sprite(LoadSprFile("./Assets/JumpMan/JumpMan.spr"), 12));
		wallTexture = Sprite(LoadSprFile("./Assets/JumpMan/Texture/wall.spr"),1);
		coinTexture = Sprite(LoadSprFile("./Assets/JumpMan/Texture/Coin.spr"), 1);

		gameOverTexture = Sprite(LoadSprFile("./Assets/JumpMan/Texture/gameover.spr"), 1);

		BGM = new Audio("./Assets/JumpMan/Music/BGM.ogg", Music);
		gameOverMusic = new Audio("./Assets/JumpMan/Music/gameover.mp3", Chunk);

		//加载地图
		currentMap = new LevelMap(LoadMap("./Assets/JumpMan/Map/level.map"));
		
		deltaSize = (float)currentMap->h / (float)pixelHeight;
		deltaSize *= 0.25;

		for (int i = 0;i < currentMap->w;i++) 
		{
			for (int j = 0; j < currentMap->h;j++)
			{
				if (currentMap->level[j*currentMap->w+i]==63)
				{
					player->position.x = i;
					player->position.y = j;
				}

				if (currentMap ->level[j*currentMap->w+i] == 31)
				{
					//如果是白云的化
				}

			}
		}

	}

public:

	void Draw() 
	{
		for (int x = 0;x<pixelWidth;x++) 
		{
			for (int y = 0;y<pixelHeight;y++) 
			{

				Uint32 color = RGB(156,252,240);

				Vector<int> pixelPos;
				
				pixelPos.x = (player->position.x - 0.5 * pixelWidth * deltaSize + x * deltaSize);
				pixelPos.y = (player->position.y - 0.5 * pixelHeight * deltaSize + y * deltaSize);

				//开始计算
				Uint8 hitByte = 0;
				hitByte = currentMap->level[
					pixelPos.y*currentMap->w+
					pixelPos.x
				];
				float testX = (player->position.x - 0.5 * pixelWidth * deltaSize + x * deltaSize);
				float testY = (player->position.y - 0.5 * pixelHeight * deltaSize + y * deltaSize);
				Vector<float> simple;
				simple.x = testX - (int)(testX);
				simple.y = testY - (int)(testY);
				if (hitByte == 255)
				{
					//进行物品贴图
					color = wallTexture.GetTransposeColor(simple.y,simple.x);
				}
				if (hitByte == 127)
				{
					if (coinTexture.GetTransposeColor(simple.y, simple.x)!=BLACK)
					{
						color = coinTexture.GetTransposeColor(simple.y, simple.x);
					}
				}
				renderer.DrawPixel(x, y, color);

			}
		}
		
		//绘制白云

		//绘制玩家
		player->sprite.DrawSprite(pixelWidth/2,pixelHeight/2,true,!player->isLeft);

	}

	void Start() override
	{
		BGM->Play(true);
	}

	void Update() override
	{
		
		if (gameOver)
		{
			//先黑屏

			//BGM.Stop();
			//gameOverMusic.Play(false);

			//float gameoverPanelDelta = gameOverTexture.w / pixelWidth;

			for (int x = 0; x < pixelWidth; x++)
			{
				for (int y = 0; y < pixelHeight; y++)
				{

					float testX = (float)(x) / (float)(pixelWidth);
					float testY = (float)(y) / (float)(pixelHeight);

					Uint32 color = gameOverTexture.GetColor(testX,testY);
					renderer.DrawPixel(x, y, color);
				}
			}

			delete currentMap;
			//加载地图
			currentMap = new LevelMap(LoadMap("./Assets/JumpMan/Map/level.map"));
			
			for (int i = 0; i < currentMap->w; i++)
			{
				for (int j = 0; j < currentMap->h; j++)
				{
					if (currentMap->level[j * currentMap->w + i] == 63)
					{
						player->position.x = i;
						player->position.y = j;
					}

					if (currentMap->level[j * currentMap->w + i] == 31)
					{
						//如果是白云的化
					}

				}
			}

			if (inputSystem.GetKeyDown(KeyK))
			{
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



bool ColliderWithMap(Uint8 byte, Vector<float> colliderPos)
{
	//检测玩家和地图的碰撞
	Uint8 hitByte = currentMap->level[(int)(colliderPos.y) * currentMap->w + (int)colliderPos.x];
	if (hitByte == byte)
	{
		return true;
	}
	return false;
}

int main(int argc,char * argv[]) 
{
	Mario mario(1920 / 2, 1080 / 2, NoaGameEngine::WindowMode, "Mario");
	mario.Run();
	return 0;
}
