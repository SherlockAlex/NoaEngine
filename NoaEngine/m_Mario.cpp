#define MARIO_GAME
#ifdef MARIO_GAME

#include "NoaEngine.h"

/// <summary>
/// 类似《超级马里奥》的游戏
/// </summary>

using namespace noa;

extern vector<GameObject*> gameObjects;

LevelMap* currentMap;

int coinCount = 0;
bool gameOver = false;

static bool ColliderWithMap(Uint8 byte, Vector<float> colliderPos);

static Audio * BGM;
static Audio * gameOverMusic;

//实现tileMap
unordered_map <Uint8, Sprite> tileMap =
{
	{0,Sprite(LoadSprFile("./Assets/JumpMan/Texture/wall.spr"),1)},
	//{127,Sprite(LoadSprFile("./Assets/JumpMan/Texture/Coin.spr"), 1)}
};

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

class Player :public GameObject,public Rigidbody
{
private:
	function<void()> playerControlCallback;

public:
	float speed = 10;
	bool isJump = false;
	float jumpTime = 0.0;
	float jumpSpeed = 0;
	bool isLeft = false;
	//bool isGrounded = false;

	Vector<float> colliderSize;
	Audio coinSFX = Audio("./Assets/JumpMan/Music/coin.mp3", Chunk);
	Audio jumpSFX = Audio("./Assets/JumpMan/Music/jump.mp3", Chunk);

	//Animator
	Animator* currentAnimatorState = nullptr;

	Animator* idle = new Animator(5);
	Animator* run = new Animator(10);
	Animator* jump = new Animator(10);

public:
	Player(Sprite sprite):GameObject(sprite),Rigidbody(&(this->position))
	{
		//玩家的构造函数
		colliderSize.x = 0.6;
		colliderSize.y = 0.7;

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
			Vector<float> pos;
			pos.x = position.x - colliderSize.x;
			pos.y = position.y + colliderSize.y;
			if (ColliderWithMap(0, pos))
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
			//AddForce(Vector<float>(1, 0), ContinuousForce);
			//Debug("Move right");
			isLeft = false;
			position.x += deltaTime * speed;
			Vector<float> pos;
			pos.x = position.x + colliderSize.x;
			pos.y = position.y + colliderSize.y;
			if (ColliderWithMap(0, pos))
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

		if (inputSystem.GetKeyHold(KeyK)&&isGrounded)
		{
			if (!isJump)
			{
				Debug("Jump");
				jumpSFX.Play(false);
				currentAnimatorState = jump;
				AddForce(Vector<float>(0, -15), Impulse);
				isJump = true;
			}
			
		}

		//下落的碰撞检测

		Vector<float> pos;
		pos.x = position.x;
		pos.y = position.y + colliderSize.y;
		if (isGrounded&&ColliderWithMap(0, pos))
		{
			position.y -= 0.05;
		}
		pos.x = position.x - colliderSize.x;
		pos.y = position.y + colliderSize.y;
		if (isGrounded && ColliderWithMap(0, pos))
		{
			position.y -= 0.05;
		}
		pos.x = position.x + colliderSize.x;
		pos.y = position.y + colliderSize.y;
		if (isGrounded && ColliderWithMap(0, pos))
		{
			position.y -= 0.05;
		}

		//检测头顶是否顶砖块
		pos.x = position.x;
		pos.y = position.y - colliderSize.y;
		if (ColliderWithMap(0, pos))
		{
			position.y += velocity.y * deltaTime;
			velocity.y = 5;
		}
		pos.x = position.x - colliderSize.x;
		pos.y = position.y - colliderSize.y;
		if (ColliderWithMap(0, pos))
		{
			position.y += velocity.y * deltaTime;
			velocity.y = 5;
		}
		pos.x = position.x + colliderSize.x;
		pos.y = position.y - colliderSize.y;
		if (ColliderWithMap(0, pos))
		{
			position.y += velocity.y * deltaTime;
			velocity.y = 5;
		}

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

		const Uint8 hitItem = currentMap->level[testPos.y * currentMap->w + testPos.x];
		if (hitItem == 127) 
		{
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

		const SpriteFile nextFrame = move(currentAnimatorState->GetCurrentFrameImage());
		sprite.UpdateImage(nextFrame);

		isGrounded = false;

		if ((currentMap->level[
			(int)(position.y+ colliderSize.y+0.1) * currentMap->w + (int)position.x] 
			== 0)|| 
			(currentMap->level[
				(int)(position.y + colliderSize.y + 0.1) * currentMap->w + (int)(position.x - colliderSize.x)]
				== 0) || (currentMap->level[
					(int)(position.y + colliderSize.y + 0.1) * currentMap->w + (int)(position.x + colliderSize.x)]
					== 0))
		{
			isGrounded = true;
		}
		
		//Debug("isGrounded:"+to_string(isGrounded));
				

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
		player = new Player(Sprite(LoadSprFile("./Assets/JumpMan/JumpMan.spr"), 11));
		gameOverTexture = Sprite(LoadSprFile("./Assets/JumpMan/Texture/gameover.spr"), 1);
		skyboxTexture = Sprite(LoadSprFile("./Assets/JumpMan/Texture/skybox.spr"), 1);
		BGM = new Audio("./Assets/JumpMan/Music/BGM.ogg", Music);
		gameOverMusic = new Audio("./Assets/JumpMan/Music/gameover.mp3", Chunk);

		//加载地图
		//currentMap = new LevelMap(LoadMap("./Assets/JumpMan/Map/level.map"));
		currentMap = new LevelMap(LoadMapFromCSV("./Assets/JumpMan/Map/level1.csv"));

		deltaSize = (float)currentMap->h / (float)pixelHeight;
		deltaSize *= 0.25;

		for (int i = 0;i < currentMap->w;i++) 
		{
			for (int j = 0; j < currentMap->h;j++)
			{

				if (currentMap ->level[j*currentMap->w+i] == 31)
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
		for (int x = 0;x<pixelWidth;x++) 
		{
			for (int y = 0;y<pixelHeight;y++) 
			{
				
				pixelPos.x = (player->position.x - 0.5 * pixelWidth * deltaSize + x * deltaSize);
				pixelPos.y = (player->position.y - 0.5 * pixelHeight * deltaSize + y * deltaSize);

				//开始计算
				Uint8 hitByte = 10;
				if (pixelPos.y<=currentMap->h&&pixelPos.y>=0&&pixelPos.x<=currentMap->w&&pixelPos.x>=0)
				{
					hitByte = currentMap->level[
						pixelPos.y * currentMap->w +
							pixelPos.x
					];
				}
				
				float testX = (player->position.x - 0.5 * pixelWidth * deltaSize + x * deltaSize);
				float testY = (player->position.y - 0.5 * pixelHeight * deltaSize + y * deltaSize);
				
				simple.x = testX - (int)(testX);
				simple.y = testY - (int)(testY);

				Uint32 color = 0;

				if (hitByte==0)
				{
					color = tileMap[hitByte].GetTransposeColor(simple.y, simple.x);
				}

				if (color == BLACK)
				{
					//渲染天空
					simple.x = x / (float)pixelWidth;
					simple.y = y / (float)pixelHeight;
					color = skyboxTexture.GetTransposeColor(simple.y, simple.x);
				}

				renderer.DrawPixel(x, y, color);

				

			}
		}
		//绘制玩家
		player->sprite.DrawSprite(pixelWidth/2,pixelHeight/2,true,!player->isLeft);



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
			//先黑屏
			//canDraw = false;
			//BGM.Stop();
			//gameOverMusic.Play(false);

			//float gameoverPanelDelta = gameOverTexture.w / pixelWidth;

			/*for (int x = 0; x < pixelWidth; x++)
			{
				for (int y = 0; y < pixelHeight; y++)
				{

					float testX = (float)(x) / (float)(pixelWidth);
					float testY = (float)(y) / (float)(pixelHeight);

					Uint32 color = gameOverTexture.GetColor(testX,testY);
					renderer.DrawPixel(x, y, color);
				}
			}*/

			gameOverTexture.DrawSpriteFull();

			//加载地图
			delete currentMap;

			currentMap = nullptr;

			if (inputSystem.GetKeyDown(KeyK))
			{
				
				currentMap = new LevelMap(LoadMapFromCSV("./Assets/JumpMan/Map/level1.csv"));
				for (int i = 0; i < currentMap->w; i++)
				{
					for (int j = 0; j < currentMap->h; j++)
					{

						if (currentMap->level[j * currentMap->w + i] == 31)
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



bool ColliderWithMap(Uint8 byte, Vector<float> colliderPos)
{
	//检测玩家和地图的碰撞
	if (currentMap == nullptr)
	{
		return false;
	}
	Uint8 hitByte = currentMap->level[(int)(colliderPos.y) * currentMap->w + (int)colliderPos.x];
	if (hitByte == byte)
	{
		return true;
	}
	return false;
}

int main(int argc,char * argv[]) 
{
	Mario mario(1920/2, 1080/2, NoaGameEngine::WindowMode, "Mario");
	mario.Run();
	return 0;
}

#endif