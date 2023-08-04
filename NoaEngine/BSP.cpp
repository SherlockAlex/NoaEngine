#include "DOOMAssets.h"
#include "NoaEngine.h"

struct BSPNode {
	int x, y, width, height; // 节点在地图中的位置和大小
	BSPNode* left; // 左子树
	BSPNode* right; // 右子树
};

static LevelMap currentMap;
static Player player;
static float distanceToCollider = 0.0f;
#define Animation
static bool isPlayAnimation = false;
static float i = 0;
static float animationSpeed = 7;
static Animator gunAniamtor;
static Sprite gunSprite(238, 258, 4, gunColor1);
#define AudioManager
static Audio* gunShotChunk;
static Audio* themeMusic;
#define Function
static Ray RayCastHit(int pixelX, Player& player, LevelMap& map);//射线检测
static void GameInput();
static void DrawMap();
#define Wall
static float heightOfWall = 1.0f;					//墙壁高度

static void GameStart(void)
{
}

static void GameUpdate(void)
{
	GameInput();
	DrawMap();
}

BSPNode* BuildBSP(int x, int y, int width, int height, int depth) {
	if (depth <= 0 || width <= 2 || height <= 2) {
		return nullptr; // 达到终止条件，返回节点为空
	}

	BSPNode* node = new BSPNode();
	node->x = x;
	node->y = y;
	node->width = width;
	node->height = height;

	// 按照某种规则分割节点，例如垂直或水平分割
	bool splitVertically = (rand() % 2 == 0); // 随机决定是水平分割还是垂直分割
	int splitPos = (splitVertically) ? (x + rand() % (width - 2)) : (y + rand() % (height - 2));

	if (splitVertically) {
		node->left = BuildBSP(x, y, splitPos - x + 1, height, depth - 1); // 创建左子树
		node->right = BuildBSP(splitPos + 1, y, x + width - splitPos - 1, height, depth - 1); // 创建右子树
	}
	else {
		node->left = BuildBSP(x, y, width, splitPos - y + 1, depth - 1); // 创建左子树
		node->right = BuildBSP(x, splitPos + 1, width, y + height - splitPos - 1, depth - 1); // 创建右子树
	}

	return node;
}

static Ray RayCastHit(
	int pixelX,			//像素点横坐标
	Player& player,		//玩家对象引用
	LevelMap& map		//当前关卡地图引用
)
{
	//射线投射算法
	Ray ray;
	ray.distance = 0.0f;
	ray.angle = player.angle -
		player.FOV * (0.5f - (float)(pixelX) / (float)(surfaceWidth));
	const float rayForwordStep = 0.03f;
	const float eyeX = sinf(ray.angle);
	const float eyeY = cosf(ray.angle);

	while (!ray.isHitDoor && !ray.isHitWall && ray.distance < player.viewDepth)
	{
		ray.distance += rayForwordStep;

		const float floatHitPointX = player.position.x + ray.distance * eyeX;
		const float floatHitPointY = player.position.y + ray.distance * eyeY;

		const int intHitPointX = (int)floatHitPointX;
		const int intHitPointY = (int)floatHitPointY;

		if (intHitPointX < 0 || intHitPointX >= map.w || intHitPointY < 0 || intHitPointY >= map.h)
		{
			ray.isHitWall = true;
			ray.distance = player.viewDepth;
			continue;
		}

		const char hitChar = map.level[intHitPointY * map.w + intHitPointX];
		if (hitChar == '#' || (hitChar == '*'))
		{

			if (hitChar == '#')
			{
				ray.isHitWall = true;
			}
			else if (hitChar == '*')
			{
				ray.isHitDoor = true;
			}

			const float fBlockMidX = (float)intHitPointX + 0.5f;
			const float fBlockMidY = (float)intHitPointY + 0.5f;
			const float fTestAngle = atan2f((floatHitPointY - fBlockMidY), (floatHitPointX - fBlockMidX));

			if (fTestAngle >= -PI * 0.25f && fTestAngle < PI * 0.25f)
			{
				ray.simple.x = floatHitPointY - (float)intHitPointY;

			}
			if (fTestAngle >= PI * 0.25f && fTestAngle < PI * 0.75f)
			{
				ray.simple.x = floatHitPointX - (float)intHitPointX;
			}
			if (fTestAngle < -PI * 0.25f && fTestAngle >= -PI * 0.75f)
			{
				ray.simple.x = floatHitPointX - (float)intHitPointX;
			}
			if (fTestAngle >= PI * 0.75f || fTestAngle < -PI * 0.75f)
			{
				ray.simple.x = floatHitPointY - (float)intHitPointY;
			}
		}
	}

	ray.distance = ray.distance * cosf(player.angle - ray.angle);

	return ray;

}

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
		//实现射击效果
		//需要每一帧去播放动画
		if (!isPlayAnimation)
		{
			isPlayAnimation = true;
			//发射子弹，是否命中敌人
			gunShotChunk->Play(false);
		}

	}
	if (isPlayAnimation)
	{
		i = i + deltaTime * animationSpeed;
		if (i > 6)
		{
			i = 0;
			isPlayAnimation = false;
		}
		gunAniamtor.Play(i);
		gunSprite.UpdateImage(gunAniamtor.GetCurrentFrameImage());
	}

	if (GetKeyHold(KeyL))
	{
		InteractWithObject('*');
	}

	if (GetKeyHold(KeyM))
	{
		heightOfWall += deltaTime;
		if (heightOfWall > 1)
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

void DrawMap() {
	// 循环绘制地图中的每个像素
	for (int pixelY = 0; pixelY < screenHeight; pixelY++) {
		// 计算当前行与投影平面的相对高度
		float fRayDirY0 = player.dirY - player.planeY;
		float fRayDirX0 = player.dirX - player.planeX;
		float fRayDirY1 = player.dirY + player.planeY;
		float fRayDirX1 = player.dirX + player.planeX;

		// 计算当前像素在相对高度中的比例
		float fP = (float)(2 * pixelY - screenHeight) / screenHeight;

		// 计算射线起点到当前像素的方向向量
		float fRayDirX = fRayDirX0 + (fRayDirX1 - fRayDirX0) * fP;
		float fRayDirY = fRayDirY0 + (fRayDirY1 - fRayDirY0) * fP;

		// 计算射线起点与当前像素的位置
		int mapX = (int)player.posX;
		int mapY = (int)player.posY;

		// 计算射线的步长和方向
		float deltaDistX = std::abs(1 / fRayDirX);
		float deltaDistY = std::abs(1 / fRayDirY);
		int stepX, stepY;

		// 根据射线的方向设置步长和方向
		if (fRayDirX < 0) {
			stepX = -1;
			sideDistX = (player.posX - mapX) * deltaDistX;
		}
		else {
			stepX = 1;
			sideDistX = (mapX + 1.0 - player.posX) * deltaDistX;
		}
		if (fRayDirY < 0) {
			stepY = -1;
			sideDistY = (player.posY - mapY) * deltaDistY;
		}
		else {
			stepY = 1;
			sideDistY = (mapY + 1.0 - player.posY) * deltaDistY;
		}

		// 通过DDA算法进行射线投射
		while (true) {
			// 根据射线的步长选择X向或Y向走一步
			if (sideDistX < sideDistY) {
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else {
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}

			// 根据地图的高度值绘制贴图
			if (mapArray[mapX][mapY] == '#') {
				// 高度值为1的物体，使用#表示
				DrawPixel(pixelX, pixelY, WallTexture);
			}
			else if (mapArray[mapX][mapY] == '*') {
				// 高度值为1的物体，使用*表示
				DrawPixel(pixelX, pixelY, ObjectTexture);
			}
			else if (mapArray[mapX][mapY] == '@') {
				// 高度值为2的物体，使用@表示
				DrawPixel(pixelX, pixelY, CeilingTexture);
			}
			else {
				// 绘制天花板和地板的贴图
				// ...
			}
		}

		// ...
	}
}


//static void DrawMap()
//{
//	for (int x = 0; x < surfaceWidth; x++)
//	{
//		Ray ray = RayCastHit(x, player, currentMap);
//
//
//		float ceiling = surfaceHeight * 0.5f - (float)(surfaceHeight * heightOfWall) / ray.distance;
//		float ceilingSimpleY = surfaceHeight * 0.5f - (float)(surfaceHeight) / ray.distance;
//		float floor = surfaceHeight - ceiling;
//		float floorSimpleY = surfaceHeight - ceilingSimpleY;
//
//		Uint32 color = BLACK;
//
//		float shadowOfWall = 1;
//
//		float sharkCamera = 75 * (sinf(1.5f * player.position.x) + sinf(1.5f * player.position.y));
//		sharkCamera = sharkCamera / ray.distance;		//镜头晃动
//
//		for (int y = 0; y < surfaceHeight; y++)
//		{
//			if (y <= ceiling + sharkCamera)
//			{
//				//绘制天花板
//				color = RGB(127, 127, 127);
//			}
//			else if (y > ceiling + sharkCamera && y <= floor + sharkCamera)
//			{
//				ray.simple.y = ((float)y - (float)(ceilingSimpleY + sharkCamera)) / ((float)floorSimpleY - (float)ceilingSimpleY);
//				color = GetSpriteColor(ray.simple.y, ray.simple.x, 256, 256, wall1Color);
//
//				if (ray.isHitDoor)
//				{
//					//绘制门
//					color = GetSpriteColor(ray.simple.y, ray.simple.x, 128, 128, doorColor);
//				}
//
//				color = RGB(GetRValue(color) * shadowOfWall, GetGValue(color) * shadowOfWall, GetBValue(color) * shadowOfWall);
//				//计算阴影,计算法向
//			}
//			else {
//				//绘制地板
//				float b = 1.0f - (((float)y - surfaceHeight / 2.0f) / ((float)surfaceHeight / 2.0f));
//				float deltaRayShine = (1 - b) * (1 - b);
//
//				float depth = (1 - b);
//				float fEyeX = sinf(ray.angle); // Unit vector for ray in player space
//				float fEyeY = cosf(ray.angle);
//				float fTestPointX = player.position.x + fEyeX * depth;
//				float fTestPointY = player.position.y + fEyeY * depth;
//
//				color = RGB(200, 200, 200);
//			}
//
//			DrawPixel(x, y, color);
//
//		}
//	}
//
//	int gunPosX = surfaceWidth / 2 + 20 * (sinf(player.position.x) + sinf(player.position.y));
//	int gunPosY = surfaceHeight - (int)((258.0 / 238.0) * (surfaceWidth / 4) * 0.5f) + 15 * ((sinf(2 * player.position.x) + 1) + (sinf(2 * player.position.y) + 1));
//	gunSprite.DrawSprite(gunPosX, gunPosY, true);
//	std::printf("X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f\n", player.position.x, player.position.y, player.angle, 1.0f / deltaTime);
//
//}

//SET_GAME_START(GameStart)
//SET_GAME_LOOP(GameUpdate)
//SET_GAME_WINDOW(1920 / 2, 1080 / 2, WindowMode)