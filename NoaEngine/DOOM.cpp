#define DOOM
#ifdef DOOM

#include "NoaEngine.h"
#include <map>

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
//wall
Uint32* wallColor_1;
Uint32* wallColor_2;

static void GameStart(void)
{
	//从本地加载贴图文件
	wallColor_1 = LoadTexture("./Assets/Texture/Wall/wall_1.spr");
	wallColor_2 = LoadTexture("./Assets/Texture/Wall/wall_5.spr");

	//这个是地图关卡文件
	wstring map;
	map += L"##################################";
	map += L"##################################";
	map += L"############.......###############";
	map += L"############.......####@@@@@@@@###";
	map += L"############.......####@@@@@@@@###";
	map += L"############.......####@@@@@@@@###";
	map += L"############.......####@@@@@@@@###";
	map += L"############.......####@@@@@@@@###";
	map += L"############.......###############";
	map += L"###############*##################";
	map += L"##############...#################";
	map += L"############.................#####";
	map += L"############.................#####";
	map += L"############.................#####";
	map += L"#####........................#####";
	map += L"#####........................#####";
	map += L"#####...####.................#####";
	map += L"#####...##########################";
	map += L"#####...............##############";
	map += L"#####...............####@@@@@@@###";
	map += L"#################...####@@@@@@@###";
	map += L"##################*###############";
	map += L"#################...##############";
	map += L"###......................#########";
	map += L"###......................#########";
	map += L"###..............#################";
	map += L"###..............#..............##";
	map += L"###..............*..............##";
	map += L"###..............#..............##";
	map += L"##################################";
	LevelMap firstMap(34, 30, map);
	currentMap = firstMap;

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
	player.position.x += dx * player.speed * deltaTime;
	player.position.y += dy * player.speed * deltaTime;

	const char hitChar = currentMap.level[
		(int)(player.position.y + distanceToCollider) * currentMap.w
			+ (int)(player.position.x + distanceToCollider)
	];
	if (hitChar == '#' || hitChar == '*')
	{
		player.position.x -= dx * player.speed * deltaTime;
		player.position.y -= dy * player.speed * deltaTime;
	}
}

void InteractWithObject(char interactable)
{
	player.position.x += sinf(player.angle) * player.speed * deltaTime;
	player.position.y += cosf(player.angle) * player.speed * deltaTime;

	const char hitChar = currentMap.level[
		(int)(player.position.y + distanceToCollider) * currentMap.w
			+ (int)(player.position.x + distanceToCollider)
	];
	if (hitChar == interactable)
	{
		cout << "interact" << endl;
		currentMap.level[(int)player.position.y * currentMap.w + (int)player.position.x] = '.';
	}

	player.position.x -= sinf(player.angle) * player.speed * deltaTime;
	player.position.y -= cosf(player.angle) * player.speed * deltaTime;
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
	if (GetKeyHold(KeyA)) 
	{
		player.angle -= (player.speed * 0.25f) * deltaTime;
	}
		

	// 处理顺时针旋转
	if (GetKeyHold(KeyD))
	{
		player.angle += (player.speed * 0.25f) * deltaTime;
	}

	// 处理前进移动和碰撞
	if (GetKeyHold(KeyW))
	{
		MoveAndCheckCollision(sinf(player.angle), cosf(player.angle));
	}

	// 处理后退移动和碰撞
	if (GetKeyHold(KeyS))
	{
		MoveAndCheckCollision(-sinf(player.angle), -cosf(player.angle));
	}

	// 处理左移和碰撞
	if (GetKeyHold(KeyQ))
	{
		MoveAndCheckCollision(-cosf(player.angle), sinf(player.angle));
	}

	// 处理右移和碰撞
	if (GetKeyHold(KeyE))
	{
		MoveAndCheckCollision(cosf(player.angle), -sinf(player.angle));
	}


	if (GetKeyDown(KeyJ))
	{
		cout << "fire" << endl;
		gunAniamtor.Play();
		gunSprite.UpdateImage(gunAniamtor.GetCurrentFrameImage());

	}

	//这里播放动画，但是这样的设计不利于扩展
	//我们需要一个触发器，按下的时候就播放动画
	
	/*if (isPlayAnimation)
	{
		i = i + deltaTime * animationSpeed;
		if (i > 6)
		{
			i = 0;
			isPlayAnimation = false;
		}
		gunAniamtor.Play(i);
		
	}*/

	gunSprite.UpdateImage(gunAniamtor.GetCurrentFrameImage());

	if (GetKeyHold(KeyL))
	{
		InteractWithObject('*');
	}

	if (GetKeyHold(KeyM))
	{
		heightOfWall += deltaTime;
		if (heightOfWall>1)
		{
			heightOfWall -= deltaTime;
		}
	}

	if (GetKeyHold(KeyN))
	{
		heightOfWall -= deltaTime;
		if (heightOfWall < 0)
		{
			heightOfWall += deltaTime;
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

		/*float shadowOfWall = 1 / (1 +
			ray.distance * ray.distance * ray.distance
			* ray.distance * ray.distance * 0.00002);*/

		float shadowOfWall = 1;

		float sharkCamera = 75*(sinf(1.5f*player.position.x) + sinf(1.5f*player.position.y));
		sharkCamera = sharkCamera / ray.distance;		//镜头晃动

		for (int y = 0;y<surfaceHeight;y++)
		{
			if (y<=ceiling + sharkCamera)
			{
				//绘制天花板
				color = RGB(127, 127, 127);
			}
			else if (y>ceiling + sharkCamera &&y<=floor+ sharkCamera)
			{
				ray.simple.y = ((float)y - (float)(ceilingSimpleY + sharkCamera)) / ((float)floorSimpleY - (float)ceilingSimpleY);
				color = GetSpriteColor(ray.simple.y, ray.simple.x, 256, 256, wallColor_1);

				if (ray.isHitDoor)
				{
					//绘制门
					color = GetSpriteColor(ray.simple.y, ray.simple.x, 128, 128, wallColor_2);
				}

				color = RGB(GetRValue(color) * shadowOfWall, GetGValue(color) * shadowOfWall, GetBValue(color) * shadowOfWall);
				//计算阴影,计算法向
			}
			else {
				//绘制地板
				float b = 1.0f - (((float)y - surfaceHeight / 2.0f) / ((float)surfaceHeight / 2.0f));
				float deltaRayShine = (1 - b) * (1 - b);

				float depth = (1 - b);
				float fEyeX = sinf(ray.angle); // Unit vector for ray in player space
				float fEyeY = cosf(ray.angle);
				float fTestPointX = player.position.x + fEyeX * depth;
				float fTestPointY = player.position.y + fEyeY * depth;

				/*fSimpleY = fTestPointY - (int)fTestPointY;
				fSimpleX = fTestPointX - (int)fTestPointX;
				color = GetSpriteColor(fSimpleX, fSimpleY, 32, 32, grassColor);*/
				color = RGB(200,200,200);
				//color = RGB(GetRValue(color) * deltaRayShine, GetGValue(color) * deltaRayShine, GetBValue(color) * deltaRayShine);
			}

			DrawPixel(x, y, color);

		}
	}

	int gunPosX = surfaceWidth / 2 + 20 * (sinf(player.position.x) + sinf(player.position.y));
	int gunPosY = surfaceHeight - (int)((258.0 / 238.0) * (surfaceWidth / 4) * 0.5f) + 15 * ((sinf(2 * player.position.x) + 1) + (sinf(2 * player.position.y) + 1));

	gunSprite.DrawSprite(gunPosX, gunPosY, true);

	//std::printf("X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f\n", player.position.x, player.position.y, player.angle, 1.0f / deltaTime);

}

SET_GAME_START(GameStart)
SET_GAME_LOOP(GameUpdate)
SET_GAME_WINDOW(1920/2, 1080/2, WindowMode)

#endif // DOOM