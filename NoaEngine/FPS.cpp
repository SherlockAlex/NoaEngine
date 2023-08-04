#ifdef FPS




#include "NoaEngine.h"
#include "DOOMAssets.h"


//Uint32 *gunAnimation;

typedef struct FPSPlayer {
	float fPlayerX = 15.0f;				// 角色坐标
	float fPlayerY = 5.9f;
	float fPlayerZ = 0.0f;

	float fPlayerA = 0.0f;				// 角色转角
	float fFOV = 3.14159f / 4.0f;		// 角色视野范围
	float fDepth = 30.0f;				// 可视范围
	float fSpeed = 7.0f;
}FPSPlayer;

//static char currentMap[65535];
static vector<GameObject> gameObjects;

extern float RayCostHit(
	FPSPlayer* player,	//玩家
	LevelMap* map,
	int x,				//像素点的横坐标
	bool& bHitDoor,		//是否命中墙壁
	bool& bHitWall,		//是否命中门
	float& fSimpleX,	//纹理映射坐标x
	float& fSimpleY,	//纹理映射坐标y
	float& fRayAngle	//射线的转角
);

//Mix_Music* themeMusic;
//Mix_Chunk* gunShotChunk;
//themeMusic = Mix_LoadMUS("./theme.mp3");
//gunShotChunk = Mix_LoadWAV("./shotgun.wav");
//Chunk
static Audio * gunShot;
//音乐
static Audio * themeMusic;

static LevelMap * currentLevel;

static FPSPlayer fpsPlayer;


static float distanceToCollider = 0.0f;
static bool isPlayAnimation = false;
static float i = 0;
static float animationSpeed = 7;
static Animator gunAniamtor;
static Sprite gunSprite(238,258,4,gunColor1);

void FPSPlayerInput(FPSPlayer* fpsPlayer) {

	// Handle CCW Rotation
	if (GetKeyHold(KeyA))
		fpsPlayer->fPlayerA -= (fpsPlayer->fSpeed * 0.25f) * deltaTime;

	// Handle CW Rotation
	if (GetKeyHold(KeyD))
		fpsPlayer->fPlayerA += (fpsPlayer->fSpeed * 0.25f) * deltaTime;

	// Handle Forwards movement & collision
	if (GetKeyHold(KeyW))
	{
		fpsPlayer->fPlayerX += sinf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
		fpsPlayer->fPlayerY += cosf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
		if (currentLevel->level[(int)(fpsPlayer->fPlayerY + distanceToCollider) * currentLevel->w + (int)(fpsPlayer->fPlayerX + distanceToCollider)] == '#' ||
			(currentLevel->level[(int)(fpsPlayer->fPlayerY + distanceToCollider) * currentLevel->w + (int)(fpsPlayer->fPlayerX + distanceToCollider)] == '*'))
		{
			fpsPlayer->fPlayerX -= sinf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
			fpsPlayer->fPlayerY -= cosf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
		}
	}

	// Handle backwards movement & collision
	if (GetKeyHold(KeyS))
	{
		fpsPlayer->fPlayerX -= sinf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
		fpsPlayer->fPlayerY -= cosf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
		if (currentLevel->level[(int)(fpsPlayer->fPlayerY - distanceToCollider) * currentLevel->w + (int)(fpsPlayer->fPlayerX - distanceToCollider)] == '#' ||
			(currentLevel->level[(int)(fpsPlayer->fPlayerY - distanceToCollider) * currentLevel->w + (int)(fpsPlayer->fPlayerX - distanceToCollider)] == '*'))
		{
			fpsPlayer->fPlayerX += sinf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
			fpsPlayer->fPlayerY += cosf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
		}
	}

	// Handle backwards movement & collision
	if (GetKeyHold(KeyQ))
	{
		fpsPlayer->fPlayerX -= cosf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
		fpsPlayer->fPlayerY += sinf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
		if (currentLevel->level[(int)(fpsPlayer->fPlayerY + distanceToCollider) * currentLevel->w + (int)(fpsPlayer->fPlayerX - distanceToCollider)] == '#' ||
			(currentLevel->level[(int)(fpsPlayer->fPlayerY + distanceToCollider) * currentLevel->w + (int)(fpsPlayer->fPlayerX - distanceToCollider)] == '*'))
		{
			fpsPlayer->fPlayerX += cosf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
			fpsPlayer->fPlayerY -= sinf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
		}
	}
	if (GetKeyHold(KeyE))
	{
		fpsPlayer->fPlayerX += cosf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
		fpsPlayer->fPlayerY -= sinf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
		if (currentLevel->level[(int)(fpsPlayer->fPlayerY - distanceToCollider) * currentLevel->w + (int)(fpsPlayer->fPlayerX + distanceToCollider)] == '#' ||
			(currentLevel->level[(int)(fpsPlayer->fPlayerY - distanceToCollider) * currentLevel->w + (int)(fpsPlayer->fPlayerX + distanceToCollider)] == '*'))
		{
			fpsPlayer->fPlayerX -= cosf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
			fpsPlayer->fPlayerY += sinf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;;
		}
	}

	
	if (GetKeyDown(KeyJ))
	{
		cout << "fire" << endl;
		//实现射击效果
		//需要每一帧去播放动画
		if (!isPlayAnimation)
		{
			isPlayAnimation = true;
			gunShot->Play(false);
		}
		
	}
	if (isPlayAnimation)
	{
		i = i + deltaTime* animationSpeed;
		if (i > 6)
		{
			i = 0;
			isPlayAnimation = false;
		}
		gunAniamtor.Play(i);
		gunSprite.UpdateImage(gunAniamtor.GetCurrentFrameImage());
		//gunAnimation = gunAniamtor->GetFrameImage(i);
	}

	if (GetKeyHold(KeyL))
	{
		fpsPlayer->fPlayerX += sinf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;
		fpsPlayer->fPlayerY += cosf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;
		if (currentLevel->level[(int)(fpsPlayer->fPlayerY + distanceToCollider) * currentLevel->w + (int)(fpsPlayer->fPlayerX + distanceToCollider)] == '*')
		{
			cout << "interact" << endl;
			currentLevel->level[(int)fpsPlayer->fPlayerY * currentLevel->w + (int)fpsPlayer->fPlayerX] = '.';
			//map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] = '.';

		}
		fpsPlayer->fPlayerX -= sinf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;
		fpsPlayer->fPlayerY -= cosf(fpsPlayer->fPlayerA) * fpsPlayer->fSpeed * deltaTime;
	}


}

void DrawFPSMap(FPSPlayer* player) {

	// Display Map

	Uint32 color = RED;

	for (int x = 0; x < surfaceWidth; x++)
	{
		
		float fRayAngle = 0.0f;

		bool bHitDoor = false;
		bool bHitWall = false;

		//计算贴图坐标映射
		float fSimpleX = 0.0f;
		float fSimpleY = 0.0f;



		float fDistanceToWall = RayCostHit(player,currentLevel, x,bHitDoor,bHitWall,fSimpleX,fSimpleY,fRayAngle);


		// Calculate distance to ceiling and floor
		//透视学公式，WTF
		//地板和天花板关于基准线=screenHeight/2对称
		//天花板的高度=基准线-(视高/物体距离)
		float deltaHeight = 0.0f;
		float baseLine = (surfaceHeight * 0.5f);										//准线
		int nCeiling = baseLine - surfaceHeight / ((float)fDistanceToWall);							//天花板
		int nFloor = baseLine*2 - nCeiling;

		//计算墙壁光照
		float deltaWallDepth = fDistanceToWall / player->fDepth;
		float shadowOfWall = 1 / (1 + 
			fDistanceToWall * fDistanceToWall * fDistanceToWall 
			* fDistanceToWall * fDistanceToWall * 0.00002);
		

		for (int y = 0; y < surfaceHeight; y++)
		{

			//开始进行图形绘制

			// Each Row
			if (y <= nCeiling) {
				//绘制天花板
				color = RGB(15, 15, 128);
			}
			else if (y > nCeiling+player->fPlayerZ/ fDistanceToWall && y <= nFloor) {
				//绘制墙壁
				fSimpleY = ((float)y - (float)nCeiling) / ((float)nFloor - (float)nCeiling);
				color = GetSpriteColor(fSimpleY, fSimpleX, 256, 256, wall1Color);

				if (bHitDoor)
				{
					//绘制门
					color = GetSpriteColor(fSimpleY, fSimpleX, 128, 128, doorColor);
				}

				color = RGB(GetRValue(color) * shadowOfWall, GetGValue(color) * shadowOfWall, GetBValue(color) * shadowOfWall);

				//位与(nCeiling~nCeiling+player.z/fDistance之间的像素)

			}
			else if(y>nFloor)
			{
				//实现阴影
				float b = 1.0f - (((float)y - surfaceHeight / 2.0f) / ((float)surfaceHeight / 2.0f));
				float deltaRayShine = (1 - b) * (1 - b);

				float depth = (1 - b);
				float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
				float fEyeY = cosf(fRayAngle);
				float fTestPointX = player->fPlayerX + fEyeX * depth;
				float fTestPointY = player->fPlayerY + fEyeY * depth;

				fSimpleY = fTestPointY - (int)fTestPointY;
				fSimpleX = fTestPointX - (int)fTestPointX;
				color = GetSpriteColor(fSimpleX, fSimpleY, 32, 32, grassColor);
				color = RGB(GetRValue(color) * deltaRayShine, GetGValue(color) * deltaRayShine, GetBValue(color) * deltaRayShine);

			}

			DrawPixel(x, y, color);
			//printf("drawing\n");
		}

	}

	//绘制物体到屏幕上

	for (auto& object : gameObjects)
	{
		object.positon.x += object.velocity.x * deltaTime;
		object.positon.y += object.velocity.y * deltaTime;

		float fVecX = object.positon.x - player->fPlayerX;
		float fVecY = object.positon.y - player->fPlayerY;
		float fDistanceFromPlayer = sqrtf(fVecX * fVecX + fVecY * fVecY);

		float fEyeX = sinf(player->fPlayerA);
		float fEyeY = cosf(player->fPlayerA);
		float fObjectAngle = atan2(fEyeY, fEyeX) - atan2(fVecY, fVecX);
		if (fObjectAngle < -3.14159f)
		{
			fObjectAngle += 2.0f * 3.14159f;
		}
		if (fObjectAngle > 3.14159)
		{
			fObjectAngle -= 2.0f * 3.14159f;
		}
		bool bInPlayerFOV = fabs(fObjectAngle) < player->fFOV * 0.5f;

		if (bInPlayerFOV && fDistanceFromPlayer >= 0.5f &&
			fDistanceFromPlayer < player->fDepth)
		{

			//绘制物体到屏幕上

			float fObjectCeiling = (float)(surfaceHeight * 0.5)
				- surfaceHeight / (float)fDistanceFromPlayer;
			float fObjectFloor = surfaceHeight - fObjectCeiling;

			float fObjectHeight = fObjectFloor - fObjectCeiling;
			float fObjectAspectRatio = (float)32 / (float)32;		//ͼƬ����
			float fObjectWidth = fObjectHeight / fObjectAspectRatio;

			float fMiddleOfObject = (0.5f * (fObjectAngle / (player->fFOV*0.5f)) + 0.5f)
				* (float)surfaceWidth;

			for (float lx = 0; lx < fObjectWidth; lx++)
			{
				for (float ly = 0; ly < fObjectHeight; ly++)
				{
					float objSimpleX = lx / fObjectWidth;
					float objSimpleY = ly / fObjectHeight;
					//Uint32 objColor = GetSpriteColor(objSimpleX, objSimpleY, 32, 32, bulletColor);
					Uint32 objColor = RED;
					int nObjectColumn = (int)(fMiddleOfObject + lx - (fObjectWidth * 0.5f));
					if (nObjectColumn >= 0 && nObjectColumn < surfaceWidth)
					{
						if ((int)(fObjectCeiling+ly)<0|| (int)(fObjectCeiling + ly)>=surfaceHeight
							||(nObjectColumn<0)||(nObjectColumn>=surfaceWidth)) {
							continue;
						}
						if (objColor==BLACK)
						{
							continue;
						}
						//printf("x = %d,y = %d,color = %d\n", nObjectColumn, (int)(fObjectCeiling + ly), objColor);
						DrawPixel(nObjectColumn, (int)(fObjectCeiling + ly), objColor);
					}
				}
			}
		}

	}

	int gunPosX = surfaceWidth / 2 + 20*(sinf(player->fPlayerX) + sinf(player->fPlayerY));
	int gunPosY = surfaceHeight - (int)((258.0/238.0)*(surfaceWidth/4)*0.5f)+ 15 * ((sinf(2*player->fPlayerX)+1) +(sinf(2*player->fPlayerY)+1));

	gunSprite.DrawSprite(gunPosX, gunPosY, true);
	//DrawImage(gunPosX, gunPosY, 238, 258, 4,true, gunAniamtor.GetCurrentFrameImage());

	std::printf("X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f\n", player->fPlayerX, player->fPlayerY, player->fPlayerA, 1.0f / deltaTime);
}

void DrawMiniMap(FPSPlayer* player)
{

	//currentMap[((int)player->fPlayerX + 1) * nMapWidth + (int)player->fPlayerY] = 'P';
	//printf("x = %f , y = %f\n",((int)player->fPlayerX + 1) * nScreenWidth , (int)player->fPlayerY);

	for (int x = 0; x < surfaceWidth; x++)
	{
		for (int y = 0; y < surfaceHeight; y++)
		{

			
			float deltaX = (float)currentLevel->w / (float)surfaceWidth;
			float deltaY = (float)currentLevel->w / (float)surfaceHeight;
			int testX = (int)x * deltaX;
			int testY = (int)y * deltaY;



			Uint32 color;
			color = WHITE;
			if (currentLevel->level[testY * currentLevel->w + testX] == '#')
			{
				color = RED;
			}
			if (testX == (int)player->fPlayerY && testY == ((int)player->fPlayerX + 1))
			{
				color = BLUE;
			}
			//screen[((int)fPlayerX + 1) * nScreenWidth + (int)fPlayerY] = 'P';
			//printf("x = %d,y = %d\n", testX, testY);

			//DrawPixel(x, y,color);
		}
	}
}


//射线投射算法，返回墙壁点到玩家的距离
float RayCostHit(
	FPSPlayer* player,	//玩家
	LevelMap* map,
	int x,				//像素点的横坐标
	bool& bHitDoor,
	bool& bHitWall,
	float& fSimpleX,
	float& fSimpleY,
	float& fRayAngle
) {

	// For each column, calculate the projected ray angle into world space
	fRayAngle = (player->fPlayerA - player->fFOV * 0.5f) + ((float)x / (float)surfaceWidth) * player->fFOV;

	// Find distance to wall
	float fStepSize = 0.01f;		  // Increment size for ray casting, decrease to increase										
	float fDistanceToWall = 0.0f;	  //                                      resolution

	float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
	float fEyeY = cosf(fRayAngle);

	//射线投射算法
	// Incrementally cast ray from player, along ray angle, testing for 
	// intersection with a block
	//if (!bHitWall && fDistanceToWall < fDepth)
	while ((!bHitDoor) && !bHitWall && fDistanceToWall < player->fDepth)
	{
		//计算墙壁和角色之间的距离，利用光线来测距，光纤如果没有检测到墙壁或者门，就继续前进一个单位
		fDistanceToWall += fStepSize;

		float fTestPointX = player->fPlayerX + fEyeX * fDistanceToWall;
		float fTestPointY = player->fPlayerY + fEyeY * fDistanceToWall;

		int nTestX = (int)fTestPointX;
		int nTestY = (int)fTestPointY;

		if (fDistanceToWall>player->fDepth)
		{
			fDistanceToWall = player->fDepth;
			continue;
		}

		// Test if ray is out of bounds
		if (nTestX < 0 || nTestX >= map->w || nTestY < 0 || nTestY >= map->h)
		{
			bHitWall = true;			// Just set distance to maximum depth
			fDistanceToWall = player->fDepth;
			continue;
		}
		// Ray is inbounds so test to see if the ray cell is a wall block
		if (map->level[nTestY * map->w + nTestX] == '#' ||
			map->level[nTestY * map->w + nTestX] == '*')
		{
			if (map->level[nTestY * map->w + nTestX] == '#')
			{
				// 检测到墙壁
				bHitWall = true;
			}

			if (map->level[nTestY * map->w + nTestX] == '*')
			{
				// 检测到门
				bHitDoor = true;
			}


			//计算贴图信息

			float fBlockMidX = (float)nTestX + 0.5f;
			float fBlockMidY = (float)nTestY + 0.5f;
			float fTestAngle = atan2f((fTestPointY - fBlockMidY), (fTestPointX - fBlockMidX));

			if (fTestAngle >= -3.14159f * 0.25f && fTestAngle < 3.14159f * 0.25f)
			{
				fSimpleX = fTestPointY - (float)nTestY;

			}
			if (fTestAngle >= 3.14159f * 0.25f && fTestAngle < 3.14159f * 0.75f)
			{
				fSimpleX = fTestPointX - (float)nTestX;
			}
			if (fTestAngle < -3.14159f * 0.25f && fTestAngle >= -3.14159f * 0.75f)
			{
				fSimpleX = fTestPointX - (float)nTestX;
			}
			if (fTestAngle >= 3.14159f * 0.75f || fTestAngle < -3.14159f * 0.75f)
			{
				fSimpleX = fTestPointY - (float)nTestY;
			}

			//printf("%f,%f\n", fSimpleX, fSimpleY);


		}

		//printf(" % f\n",fDistanceToWall);

	}
	//消除鱼眼效应
	fDistanceToWall *= cosf(player->fPlayerA - fRayAngle);

	return fDistanceToWall;

}

static void GameStart()
{
	// write you game init
	wstring map;
	map += L"##################################";
	map += L"##################################";
	map += L"############.......###############";
	map += L"############.......###############";
	map += L"############.......###############";
	map += L"############.......###############";
	map += L"############.......###############";
	map += L"############.......###############";
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
	map += L"#####...............##############";
	map += L"#################...##############";
	map += L"##################*###############";
	map += L"#################...##############";
	map += L"###......................#########";
	map += L"###......................#########";
	map += L"###..............#################";
	map += L"###..............#..............##";
	map += L"###..............*..............##";
	map += L"###..............#..............##";
	map += L"##################################";

	gunShot = new Audio("./shotgun.wav", Chunk);
	themeMusic = new Audio("./theme.mp3", Music);

	gunAniamtor.InsertFrameImage(gunColor1);
	gunAniamtor.InsertFrameImage(gunColor2);
	gunAniamtor.InsertFrameImage(gunColor3);
	gunAniamtor.InsertFrameImage(gunColor4);
	gunAniamtor.InsertFrameImage(gunColor5);
	gunAniamtor.InsertFrameImage(gunColor6);

	themeMusic->Play(true);
	
	currentLevel = new LevelMap(34,30,map);
	//LoadMap(map, 34, 30);
	//printf("%d\n",map.length());
}

static void GameLoop()
{
	//write your game loop

	FPSPlayerInput(&fpsPlayer);
	DrawMiniMap(&fpsPlayer);
	DrawFPSMap(&fpsPlayer);
}


//SET_GAME_START(GameStart)
//SET_GAME_LOOP(GameLoop)
//SET_GAME_WINDOW(1920/2,1080/2,WindowMode)


#endif // FPS