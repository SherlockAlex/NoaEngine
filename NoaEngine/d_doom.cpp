#define DOOM
#ifdef DOOM

#include "d_doom.h"

extern vector<GameObject*> gameObjects;

//灰度值来判断物品
//63：	玩家
//0：	墙壁
//127：	门
//31：	敌人
//255：	地板

#define Object

static LevelMap currentMap;
static Player player;
static float distanceToCollider = 0.0f;

#define AudioManager
static Audio * gunShotChunk;
static Audio * themeMusic;

static void GunChuncPlay();

#define Function
static void MouseInput();
static void GameInput();
static void DrawMap();

#define Wall
static float heightOfWall = 1.0f;					//墙壁高度

#define Texture
static Sprite wallTexture("./Assets/Texture/Wall/wall_1.spr",1);
static Sprite doorTexture("./Assets/Texture/Wall/wall_5.spr", 1);
static Sprite enimyTexture("./Assets/Texture/Enimy/Enimy.spr", 1);

vector<float> wallDistanceBuffer;

Gun * gun;

Doom game(1920/2, 1080/2, NoaGameEngine::WindowMode, "DOOM");

void GameStart(void)
{
	//这个是地图关卡文件
	const Map map = LoadMap("./Assets/Level/level_1.map");
	const LevelMap firstMap(map);
	currentMap = firstMap;

	wallDistanceBuffer = vector<float>(pixelWidth,0.0);

	//初始化物品位置
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
				new Enimy(enimyTexture, Vector<float>(i, j),&currentMap);
				Debug("添加敌人成功");
				//std::cout << "[info]:添加敌人成功" << endl;
			}

		}
	}


	//插入动画帧
	Animator* gunAnimator = new Animator(7);
	gunAnimator->LoadFromAnimatorFile("./Assets/Animator/gun.amt");
	Sprite * gunSprite = new Sprite(238, 258, 4, gunAnimator->GetCurrentFrameImage());
	gunAnimator->SetFrameEvent(1, GunChuncPlay);
	gun = new Gun(gunSprite, gunAnimator);

	//枪支声音
	gunShotChunk = new Audio("./Assets/Chunk/Gun/shotgun.wav", Chunk);
	themeMusic = new Audio("./Assets/Music/theme.mp3", Music);
	themeMusic->Play(true);

	//绑定输入事件
#ifdef __linux__
	inputSystem.BandEvent(GameInput);
#else
#ifdef _WIN64
	inputSystem.BandEvent(MouseInput);
#endif // _WIN64

#endif // DEBUG
}

void GameUpdate(void) 
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
		Debug("Interact");
		currentMap.level[(int)player.position.y * currentMap.w + (int)player.position.x] = 255;
	}

	player.position.x -= sinf(player.angle) * player.speed * game.DeltaTime();
	player.position.y -= cosf(player.angle) * player.speed * game.DeltaTime();
}

static void GunChuncPlay() 
{
	//播放音效
	Debug("Fire");
	Vector<float> bulletDir = Vector<float>(sinf(player.angle), cosf(player.angle));
	//创建一个子弹类
	Bullet* bullet = new Bullet(player.position, bulletDir,gun->damage,100, &currentMap);
	gunShotChunk->Play(false);
}

// 游戏输入
// 确定玩家的键盘输入
// 确定玩家可到墙壁或者门的距离
// 确定每个按键要执行的事件
static void MouseInput() {
	if (inputSystem.GetMouseMoveState())
	{
		Vector<float> delta = inputSystem.GetMouseMoveDelta();
		player.angle += (delta.x) * 0.3 * game.DeltaTime();
	}
}

static void GameInput() 
{
	const float distanceToCollider = 0.0f;
	//处理玩家输入问题

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
		gun->Shoot();
	}

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

//绘制游戏画面
static void DrawMap() 
{
	for (int x = 0;x<pixelWidth;x++) 
	{
		Ray ray = RayCastHit(x, player, currentMap);

		wallDistanceBuffer[x] = ray.distance;
		
		const float ceiling = pixelHeight * 0.5f - (float)(pixelHeight * heightOfWall)/ ray.distance;
		const float ceilingSimpleY = pixelHeight * 0.5f - (float)(pixelHeight) / ray.distance;
		const float floor = pixelHeight - ceiling;
		const float floorSimpleY = pixelHeight - ceilingSimpleY;

		Uint32 color = BLACK;

		const float shadowOfWall = 1 / (1 +
			ray.distance * ray.distance * ray.distance
			* ray.distance * ray.distance * 0.00002);

		float sharkCamera = 75*(sinf(1.5f*player.position.x) + sinf(1.5f*player.position.y));
		sharkCamera = sharkCamera / ray.distance;		//镜头晃动

		sharkCamera = 0;

		for (int y = 0;y< pixelHeight;y++)
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
				const float b = 1.0f - (((float)y - pixelHeight / 2.0f) / ((float)pixelHeight / 2.0f));
				const float deltaRayShine = (1 - b) * (1 - b);
				const float depth = (1 - b);

				color = WHITE;
			}
			renderer.DrawPixel(x, y, color);

		}

	}


	//绘制物品
	for (auto object : gameObjects)
	{

		const float fVecX = object->position.x - player.position.x;
		const float fVecY = object->position.y - player.position.y;

		const float fDistanceFromPlayer = sqrtf(fVecX * fVecX + fVecY * fVecY);

		const float fEyeX = sinf(player.angle);
		const float fEyeY = cosf(player.angle);
		float fObjectAngle = atan2(fEyeY, fEyeX) - atan2(fVecY, fVecX);
		if (fObjectAngle < -3.14159f)
		{
			fObjectAngle += 2.0f * 3.14159f;
		}
		if (fObjectAngle > 3.14159)
		{
			fObjectAngle -= 2.0f * 3.14159f;
		}
		const bool bInPlayerFOV = fabs(fObjectAngle) < player.FOV* 0.5f;

		if (bInPlayerFOV && fDistanceFromPlayer >= 0.5f &&
			fDistanceFromPlayer < player.viewDepth)
		{

			//绘制物体到屏幕上

			const float fObjectCeiling = (float)(pixelHeight * 0.5)
				- pixelHeight / (float)fDistanceFromPlayer;
			const float fObjectFloor = pixelHeight - fObjectCeiling;

			const float fObjectHeight = fObjectFloor - fObjectCeiling;
			const float fObjectAspectRatio = (float)object->sprite.h / (float)object->sprite.w;		
			const float fObjectWidth = fObjectHeight / fObjectAspectRatio;

			const float fMiddleOfObject = (0.5f * (fObjectAngle / (player.FOV * 0.5f)) + 0.5f)
				* (float)pixelWidth;

			for (float lx = 0; lx < fObjectWidth; lx++)
			{
				for (float ly = 0; ly < fObjectHeight; ly++)
				{
					const float objSimpleX = lx / fObjectWidth;
					const float objSimpleY = ly / fObjectHeight;
					//Uint32 objColor = GetSpriteColor(objSimpleX, objSimpleY, 32, 32, bulletColor);
					const Uint32 objColor = object->sprite.GetTransposeColor(objSimpleY,objSimpleX);
					const int nObjectColumn = (int)(fMiddleOfObject + lx - (fObjectWidth * 0.5f));
					if (nObjectColumn >= 0 && nObjectColumn < pixelWidth)
					{
						if ((int)(fObjectCeiling + ly) < 0 || (int)(fObjectCeiling + ly) >= pixelHeight
							|| (nObjectColumn < 0) || (nObjectColumn >= pixelWidth)) {
							continue;
						}
						if (objColor == BLACK||wallDistanceBuffer[nObjectColumn]<fDistanceFromPlayer)
						{
							continue;
						}
						wallDistanceBuffer[nObjectColumn] = fDistanceFromPlayer;
						//printf("x = %d,y = %d,color = %d\n", nObjectColumn, (int)(fObjectCeiling + ly), objColor);
						renderer.DrawPixel(nObjectColumn, (int)(fObjectCeiling + ly), objColor);
					}
				}
			}
		}

	}

	//绘制枪支到屏幕下方
	// deltax和deltay则是玩家自己设计
	// x = surface.width/2 + deltax
	// y = surface.height - (sprite.h/sprite.w)*(surface.width/4) + deltay
	//

	Vector<int> offset;
	offset.x = 20 * (sinf(player.position.x) + sinf(player.position.y));
	offset.y = 15 * ((sinf(2 * player.position.x) + 1) + (sinf(2 * player.position.y) + 1));

	gun->RenderGun(offset);

}

int main(int argc, char* argv[])
{
	
	return game.Run();
}

#endif // DOOM