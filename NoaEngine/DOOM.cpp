#define DOOM
#ifdef DOOM

#include "NoaEngine.h"
#include "Animator.h"
#include "Sprite.h"
#include <map>

static void GameStart(void);
static void GameUpdate(void);

NoaGameEngine game(1920/2, 1080 / 2, NoaGameEngine::WindowMode, (char *)"DOOM", GameStart, GameUpdate);

#define Object
static vector<GameObject> gameObjects;
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

static void GameStart(void)
{
	//从本地加载贴图文件
	//wallColor_1 = LoadTexture("./Assets/Texture/Wall/wall_1.spr");
	//wallColor_2 = LoadTexture("./Assets/Texture/Wall/wall_5.spr");

	//这个是地图关卡文件
	const Map map = LoadMap("./Assets/Level/level_1.map");
	LevelMap firstMap(map);
	currentMap = firstMap;

	//设置玩家位置
	for (int i=0;i<map.w;i++)
	{
		for (int j=0;j<map.h;j++) 
		{
			if (map.image[j*map.w+i]==63)
			{
				player.position.x = i;
				player.position.y = j;
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
	player.position.x += dx * player.speed * game.deltaTime;
	player.position.y += dy * player.speed * game.deltaTime;

	const char hitChar = currentMap.level[
		(int)(player.position.y + distanceToCollider) * currentMap.w
			+ (int)(player.position.x + distanceToCollider)
	];
	if (hitChar == 0 || hitChar == 127)
	{
		player.position.x -= dx * player.speed * game.deltaTime;
		player.position.y -= dy * player.speed * game.deltaTime;
	}
}

void InteractWithObject(char interactable)
{
	player.position.x += sinf(player.angle) * player.speed * game.deltaTime;
	player.position.y += cosf(player.angle) * player.speed * game.deltaTime;

	const char hitChar = currentMap.level[
		(int)(player.position.y + distanceToCollider) * currentMap.w
			+ (int)(player.position.x + distanceToCollider)
	];
	if (hitChar == interactable)
	{
		cout << "interact" << endl;
		currentMap.level[(int)player.position.y * currentMap.w + (int)player.position.x] = 255;
	}

	player.position.x -= sinf(player.angle) * player.speed * game.deltaTime;
	player.position.y -= cosf(player.angle) * player.speed * game.deltaTime;
}

static void GunChuncPlay() 
{
	//播放音效
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
	 
	// 处理逆时针旋转
	if (inputSystem.GetKeyHold(KeyA)) 
	{
		player.angle -= (player.speed * 0.25f) * game.deltaTime;
	}
		

	// 处理顺时针旋转
	if (inputSystem.GetKeyHold(KeyD))
	{
		player.angle += (player.speed * 0.25f) * game.deltaTime;
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
	if (inputSystem.GetKeyHold(KeyQ))
	{
		MoveAndCheckCollision(-cosf(player.angle), sinf(player.angle));
	}

	// 处理右移和碰撞
	if (inputSystem.GetKeyHold(KeyE))
	{
		MoveAndCheckCollision(cosf(player.angle), -sinf(player.angle));
	}


	if (inputSystem.GetKeyDown(KeyJ))
	{
		cout << "fire" << endl;
		gunAniamtor.Play();
		gunSprite.UpdateImage(gunAniamtor.GetCurrentFrameImage());

	}

	gunSprite.UpdateImage(gunAniamtor.GetCurrentFrameImage());//更新枪支图片

	if (inputSystem.GetKeyHold(KeyL))
	{
		InteractWithObject(127);
	}

	if (inputSystem.GetKeyHold(KeyM))
	{
		heightOfWall += game.deltaTime;
		if (heightOfWall>1)
		{
			heightOfWall -= game.deltaTime;
		}
	}

	if (inputSystem.GetKeyHold(KeyN))
	{
		heightOfWall -= game.deltaTime;
		if (heightOfWall < 0)
		{
			heightOfWall += game.deltaTime;
		}
	}

}

static void DrawMap() 
{
	for (int x = 0;x<surfaceWidth;x++) 
	{
		Ray ray = RayCastHit(x, player, currentMap);

		
		float ceiling = surfaceHeight * 0.5f - (float)(surfaceHeight* heightOfWall)/ ray.distance;
		float ceilingSimpleY = surfaceHeight * 0.5f - (float)(surfaceHeight) / ray.distance;
		float floor = surfaceHeight - ceiling;
		float floorSimpleY = surfaceHeight - ceilingSimpleY;

		Uint32 color = BLACK;

		float shadowOfWall = 1 / (1 +
			ray.distance * ray.distance * ray.distance
			* ray.distance * ray.distance * 0.00002);

		//float shadowOfWall = 1;

		float sharkCamera = 75*(sinf(1.5f*player.position.x) + sinf(1.5f*player.position.y));
		sharkCamera = sharkCamera / ray.distance;		//镜头晃动

		for (int y = 0;y<surfaceHeight;y++)
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
				float b = 1.0f - (((float)y - surfaceHeight / 2.0f) / ((float)surfaceHeight / 2.0f));
				float deltaRayShine = (1 - b) * (1 - b);
				float depth = (1 - b);

				color = WHITE;
			}
			
			DrawPixel(x, y, color);

		}

	}

	//绘制枪支到屏幕下方
	int gunPosX = surfaceWidth / 2 + 20 * (sinf(player.position.x) + sinf(player.position.y));
	int gunPosY = surfaceHeight - (int)((258.0 / 238.0) * (surfaceWidth / 4) * 0.5f) + 15 * ((sinf(2 * player.position.x) + 1) + (sinf(2 * player.position.y) + 1));
	
	//将图片显示再对应位置
	gunSprite.DrawSprite(gunPosX, gunPosY, true);

	//std::printf("X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f\n", player.position.x, player.position.y, player.angle, 1.0f / deltaTime);

}

#endif // DOOM