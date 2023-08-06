﻿#define DOOM
#ifdef DOOM

#include "NoaEngine.h"
#include "Animator.h"
#include "Sprite.h"
#include <map>

//灰度值来判断物品
//63：	玩家
//0：	墙壁
//127：	门
//31：	敌人
//255：	地板

static void GameStart(void);
static void GameUpdate(void);

NoaGameEngine game(1920/2, 1080/2, NoaGameEngine::WindowMode, (char *)"DOOM", GameStart, GameUpdate);

class Bullet:public Behaviour {
	//子弹类
public:
	Vector direction;
	float speed = 0;
	int damage = 50;
	LevelMap* map = nullptr;

public:
	Bullet():Behaviour()
	{

	}

	Bullet(Vector startPos,Vector direction,float speed,LevelMap * map):Behaviour()
	{
		position = startPos;
		this->direction = direction;
		this->speed = speed;
		this->map = map;
	}

	void Start() override 
	{

	}

	void Update() override
	{
		position.x = position.x + direction.x * speed * game.DeltaTime();
		position.y = position.y + direction.y * speed * game.DeltaTime();

		cout << "[info]:子弹坐标,x = " << position.x << ",y = " << position.y << endl;

		if ((int)position.x <0||(int)position.x>=map->w || (int)position.y<0 || (int)position.y >= map->h)
		{
			cout << "[info]:子弹越界,x = " << position.x << ",y = " << position.y << endl;
			Destroy();
		}

		Uint8  hitObject = map->level[(int)position.y * map->w + (int)position.x];

		if (hitObject == 127
			|| hitObject == 0)
		{
			//命中墙壁
			
			cout << "[info]:子弹命中墙壁,x = " << position.x << ",y = " << position.y << endl;
			Destroy();
		}

		for (int i = 0;i<gameObjects.size();i++) 
		{
			if (!gameObjects[i]->isTrigger)
			{
				continue;
			}

			//计算角色和子弹之间的距离

			float deltaX = gameObjects[i]->position.x - position.x;
			float deltaY = gameObjects[i]->position.y - position.y;

			float distance = deltaX * deltaX + deltaY * deltaY;

			//执行事件
			if (distance<1)
			{
				vector<void*> other;
				other.push_back(&damage);
				gameObjects[i]->OnTriggerEnter(other);
				Destroy();
			}
		}

	}
};

class Enimy:public GameObject
{

private:
	int hp = 50;

	LevelMap* map;

public:
	Enimy(Sprite enimySprite, Vector startPosition,LevelMap * map) :GameObject(enimySprite, startPosition) {
		this->map = map;
		isTrigger = true;
	}

	void Start() override {

	}

	void Update() override {

	}

	void TakeDamage(int damage) 
	{
		cout << "[info]:角色被攻击" << endl;
		hp -= damage;
		if (hp <= 0)
		{
			cout << "[info]:角色死亡" << endl;
			Destroy();
		}
	}

	void OnTriggerEnter(vector<void*> other) override {
		if (other.size()<=0) {
			return;
		}

		TakeDamage(*(int *)other[0]);

	}

};


#define Object

static LevelMap currentMap;
static Player player;
static float distanceToCollider = 0.0f;

#define Animation
static Animator gunAniamtor(7);
static Sprite gunSprite;

#define AudioManager
static Audio * gunShotChunk;
static Audio * themeMusic;

static void GunChuncPlay();

#define Function
static void GameInput();
static void DrawMap();

#define Wall
static float heightOfWall = 1.0f;					//墙壁高度

#define Texture
//wallTexture
static Sprite wallTexture("./Assets/Texture/Wall/wall_1.spr",1);
static Sprite doorTexture("./Assets/Texture/Wall/wall_5.spr", 1);
static Sprite enimyTexture("./Assets/Texture/Enimy/Enimy.spr", 1);

vector<float> wallDistanceBuffer;

static void GameStart(void)
{
	//这个是地图关卡文件
	const Map map = LoadMap("./Assets/Level/level_1.map");
	LevelMap firstMap(map);
	currentMap = firstMap;

	wallDistanceBuffer = vector<float>(game.PixelWidth(),0.0);

	//初始化设置玩家位置
	for (int i=0;i<map.w;i++)
	{
		for (int j=0;j<map.h;j++) 
		{
			if (map.image[j*map.w+i]==63)
			{
				player.position.x = i;
				player.position.y = j;
			}

			if (map.image[j*map.w+i] == 31)
			{
				new Enimy(enimyTexture, Vector(i, j),&currentMap);
				//std::cout << "[info]:添加敌人成功" << endl;
			}

		}
	}


	//插入动画帧
	gunAniamtor.LoadFromAnimatorFile("./Assets/Animator/gun.amt");
	gunSprite = Sprite(238, 258, 4, gunAniamtor.GetCurrentFrameImage());
	gunAniamtor.SetFrameEvent(1, GunChuncPlay);

	

	gunShotChunk = new Audio("./Assets/Chunk/Gun/shotgun.wav", Chunk);
	themeMusic = new Audio("./Assets/Music/theme.mp3", Music);

	themeMusic->Play(true);

}

static void GameUpdate(void) 
{
	GameInput();
	DrawMap();
}


// 射线投射算法
// 确定墙壁到玩家的距离
// 确定命中的是墙还是门
// 确定物体的贴图坐标



void MoveAndCheckCollision(float dx, float dy)
{
	player.position.x += dx * player.speed * game.DeltaTime();
	player.position.y += dy * player.speed * game.DeltaTime();

	const char hitChar = currentMap.level[
		(int)(player.position.y + distanceToCollider) * currentMap.w
			+ (int)(player.position.x + distanceToCollider)
	];
	if (hitChar == 0 || hitChar == 127)
	{
		player.position.x -= dx * player.speed * game.DeltaTime();
		player.position.y -= dy * player.speed * game.DeltaTime();
	}
}

void InteractWithObject(char interactable)
{
	player.position.x += sinf(player.angle) * player.speed * game.DeltaTime();
	player.position.y += cosf(player.angle) * player.speed * game.DeltaTime();

	const char hitChar = currentMap.level[
		(int)(player.position.y + distanceToCollider) * currentMap.w
			+ (int)(player.position.x + distanceToCollider)
	];
	if (hitChar == interactable)
	{
		game.Debug("Interact");
		currentMap.level[(int)player.position.y * currentMap.w + (int)player.position.x] = 255;
	}

	player.position.x -= sinf(player.angle) * player.speed * game.DeltaTime();
	player.position.y -= cosf(player.angle) * player.speed * game.DeltaTime();
}

static void GunChuncPlay() 
{
	//播放音效
	game.Debug("Fire");
	Vector bulletDir = Vector(sinf(player.angle), cosf(player.angle));
	Bullet* bullet = new Bullet(player.position, bulletDir, 100, &currentMap);
	gunShotChunk->Play(false);
}

// 游戏输入
// 确定玩家的键盘输入
// 确定玩家可到墙壁或者门的距离
// 确定每个按键要执行的事件
static void GameInput() 
{
	const float distanceToCollider = 0.0f;
	//处理玩家输入问题
	
	
	if (inputSystem.GetMouseMoveState())
	{
		nVector delta = inputSystem.GetMouseMoveDelta();
		player.angle += (delta.x)*0.75 * game.DeltaTime();

	}

	// 处理前进移动和碰撞
	if (inputSystem.GetKeyHold(KeyW))
	{
		MoveAndCheckCollision(sinf(player.angle), cosf(player.angle));
	}

	// 处理后退移动和碰撞
	if (inputSystem.GetKeyHold(KeyS))
	{
		MoveAndCheckCollision(-sinf(player.angle), -cosf(player.angle));
	}

	// 处理左移和碰撞
	if (inputSystem.GetKeyHold(KeyA))
	{
		MoveAndCheckCollision(-cosf(player.angle), sinf(player.angle));
	}

	// 处理右移和碰撞
	if (inputSystem.GetKeyHold(KeyD))
	{
		MoveAndCheckCollision(cosf(player.angle), -sinf(player.angle));
	}


	if (inputSystem.GetKeyDown(KeyJ)||inputSystem.GetMouseButton(LeftButton))
	{
		//鼠标左键按下
		

		gunAniamtor.Play();
		gunSprite.UpdateImage(gunAniamtor.GetCurrentFrameImage());

	}

	gunSprite.UpdateImage(gunAniamtor.GetCurrentFrameImage());//更新枪支图片

	if (inputSystem.GetKeyHold(KeyE))
	{
		InteractWithObject(127);
	}

	if (inputSystem.GetKeyHold(KeyM))
	{
		heightOfWall += game.DeltaTime();
		if (heightOfWall>1)
		{
			heightOfWall -= game.DeltaTime();
		}
	}

	if (inputSystem.GetKeyHold(KeyN))
	{
		heightOfWall -= game.DeltaTime();
		if (heightOfWall < 0)
		{
			heightOfWall += game.DeltaTime();
		}
	}

}

static void DrawMap() 
{
	for (int x = 0;x<game.PixelWidth();x++) 
	{
		Ray ray = RayCastHit(x, player, currentMap);

		wallDistanceBuffer[x] = ray.distance;
		
		const float ceiling = game.PixelHeight() * 0.5f - (float)(game.PixelHeight() * heightOfWall)/ ray.distance;
		const float ceilingSimpleY = game.PixelHeight() * 0.5f - (float)(game.PixelHeight()) / ray.distance;
		const float floor = game.PixelHeight() - ceiling;
		const float floorSimpleY = game.PixelHeight() - ceilingSimpleY;

		Uint32 color = BLACK;

		const float shadowOfWall = 1 / (1 +
			ray.distance * ray.distance * ray.distance
			* ray.distance * ray.distance * 0.00002);

		//float shadowOfWall = 1;

		float sharkCamera = 75*(sinf(1.5f*player.position.x) + sinf(1.5f*player.position.y));
		sharkCamera = sharkCamera / ray.distance;		//镜头晃动

		for (int y = 0;y< game.PixelHeight();y++)
		{
			if (y<=ceiling + sharkCamera)
			{
				//绘制天花板
				color = RGB(63, 63, 63);
				//color = RGB(GetRValue(color) * deltaRayShine, GetGValue(color) * deltaRayShine, GetBValue(color) * deltaRayShine);
			}
			else if (y>ceiling + sharkCamera &&y<=floor+ sharkCamera)
			{
				ray.simple.y = ((float)y - (float)(ceilingSimpleY + sharkCamera)) / ((float)floorSimpleY - (float)ceilingSimpleY);
				//color = GetSpriteColor(ray.simple.y, ray.simple.x, 256, 256, wallColor_1);
				color = wallTexture.GetColor(ray.simple.y, ray.simple.x);
				if (ray.isHitDoor)
				{
					//绘制门
					//color = GetSpriteColor(ray.simple.y, ray.simple.x, 128, 128, wallColor_2);
					color = doorTexture.GetColor(ray.simple.y, ray.simple.x);
				}

				color = RGB(GetRValue(color) * shadowOfWall, GetGValue(color) * shadowOfWall, GetBValue(color) * shadowOfWall);
				//计算阴影,计算法向
			}
			else {
				//绘制地板
				//根据ray.simple来计算地板贴图坐标
				//利用地板和墙壁的位置关系，以及ray.angle还有ray.distance还有ray.simple来获取地板贴图坐标
				const float b = 1.0f - (((float)y - game.PixelHeight() / 2.0f) / ((float)game.PixelHeight() / 2.0f));
				const float deltaRayShine = (1 - b) * (1 - b);
				const float depth = (1 - b);

				color = WHITE;
			}
			game.renderer.DrawPixel(x, y, color);

		}

	}


	//绘制物品
	for (auto object : gameObjects)
	{

		float fVecX = object->position.x - player.position.x;
		float fVecY = object->position.y - player.position.y;

		float fDistanceFromPlayer = sqrtf(fVecX * fVecX + fVecY * fVecY);

		float fEyeX = sinf(player.angle);
		float fEyeY = cosf(player.angle);
		float fObjectAngle = atan2(fEyeY, fEyeX) - atan2(fVecY, fVecX);
		if (fObjectAngle < -3.14159f)
		{
			fObjectAngle += 2.0f * 3.14159f;
		}
		if (fObjectAngle > 3.14159)
		{
			fObjectAngle -= 2.0f * 3.14159f;
		}
		bool bInPlayerFOV = fabs(fObjectAngle) < player.FOV* 0.5f;

		if (bInPlayerFOV && fDistanceFromPlayer >= 0.5f &&
			fDistanceFromPlayer < player.viewDepth)
		{

			//绘制物体到屏幕上

			float fObjectCeiling = (float)(game.PixelHeight() * 0.5)
				- game.PixelHeight() / (float)fDistanceFromPlayer;
			float fObjectFloor = game.PixelHeight() - fObjectCeiling;

			float fObjectHeight = fObjectFloor - fObjectCeiling;
			float fObjectAspectRatio = (float)object->sprite.h / (float)object->sprite.w;		
			float fObjectWidth = fObjectHeight / fObjectAspectRatio;

			float fMiddleOfObject = (0.5f * (fObjectAngle / (player.FOV * 0.5f)) + 0.5f)
				* (float)game.PixelWidth();

			for (float lx = 0; lx < fObjectWidth; lx++)
			{
				for (float ly = 0; ly < fObjectHeight; ly++)
				{
					float objSimpleX = lx / fObjectWidth;
					float objSimpleY = ly / fObjectHeight;
					//Uint32 objColor = GetSpriteColor(objSimpleX, objSimpleY, 32, 32, bulletColor);
					Uint32 objColor = object->sprite.GetTransposeColor(objSimpleY,objSimpleX);
					int nObjectColumn = (int)(fMiddleOfObject + lx - (fObjectWidth * 0.5f));
					if (nObjectColumn >= 0 && nObjectColumn < game.PixelWidth())
					{
						if ((int)(fObjectCeiling + ly) < 0 || (int)(fObjectCeiling + ly) >= game.PixelHeight()
							|| (nObjectColumn < 0) || (nObjectColumn >= game.PixelWidth())) {
							continue;
						}
						if (objColor == BLACK||wallDistanceBuffer[nObjectColumn]<fDistanceFromPlayer)
						{
							continue;
						}
						wallDistanceBuffer[nObjectColumn] = fDistanceFromPlayer;
						//printf("x = %d,y = %d,color = %d\n", nObjectColumn, (int)(fObjectCeiling + ly), objColor);
						game.renderer.DrawPixel(nObjectColumn, (int)(fObjectCeiling + ly), objColor);
					}
				}
			}
		}

	}

	//绘制枪支到屏幕下方
	const int gunPosX = game.PixelWidth() / 2 + 20 * (sinf(player.position.x) + sinf(player.position.y));
	const int gunPosY = game.PixelHeight() - (int)((258.0 / 238.0) * (game.PixelWidth() / 4) * 0.5f) + 15 * ((sinf(2 * player.position.x) + 1) + (sinf(2 * player.position.y) + 1));
	
	//将图片显示再对应位置
	gunSprite.DrawSprite(gunPosX, gunPosY, true);

	//std::printf("X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f\n", player.position.x, player.position.y, player.angle, 1.0f / game.DeltaTime());

}

#endif // DOOM