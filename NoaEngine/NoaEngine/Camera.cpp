#include "Actor.h"
#include "ActorComponent.h"
#include "Camera.h"
#include "Renderer.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Graphic.h"

noa::Camera::Camera(Scene* scene):Actor(scene)
{
	
}

noa::Camera::~Camera()
{

}

void noa::Camera::Update() {
	//添加camera到scene
	this->GetActiveScene()->AddCamera(this);
}

void noa::Camera::Delete(noa::Camera*& ptr)
{
	delete this;
	if (ptr != nullptr)
	{
		ptr = nullptr;
	}
}

noa::TileMapCamera::TileMapCamera(noa::Scene* scene) :noa::Camera(scene)
{

	if (scene)
	{
		this->SetTileMap(scene->GetLevelAs<TileMap>());
	}
}

noa::TileMapCamera* noa::TileMapCamera::Create(noa::Scene* scene)
{
	return NObject<TileMapCamera>::Create(scene);
}

noa::TileMapCamera& noa::TileMapCamera::SetTileScale(uint32_t w, uint32_t h)
{
	this->tileScale.x = w;
	this->tileScale.y = h;
	visibleTiles = noa::Vector<float>(
		static_cast<float>(Screen::width) / tileScale.x
		, static_cast<float>(Screen::height) / tileScale.y
	);
	return *this;
}

noa::Vector<float> tileOffset;
void noa::TileMapCamera::Render()
{


	if (follow == nullptr || tileMap == nullptr)
	{
		return;
	}

	for (auto& object : objectBufferWithRay)
	{
		object = nullptr;
	}

	position = follow->position;

	//offset表示屏幕左上角位置位于世界坐标系什么位置
	offset = std::move(position - visibleTiles*0.5f);
	if (offset.x <0) {
		offset.x =0;
	}
	else if (offset.x > tileMap->w - visibleTiles.x) {
		offset.x = tileMap->w - visibleTiles.x;
	}

	if (offset.y < 0) {
		offset.y = 0;
	}
	else if (offset.y > tileMap->h - visibleTiles.y) {
		offset.y = tileMap->h - visibleTiles.y;
	}

	tileOffset.x = (offset.x - (int)offset.x) * tileScale.x;
	tileOffset.y = (offset.y - (int)offset.y) * tileScale.y;

	//绘制地图到屏幕上
	for (int x = -2; x < visibleTiles.x + 2; x++)
	{
		for (int y = -2; y < visibleTiles.y + 2; y++)
		{
			if (tileMap->layers.empty())
			{
				continue;
			}

			const int tilePosX = static_cast<int>(x + offset.x);
			const int tilePosY = static_cast<int>(y + offset.y);

			const float tileDrawPosX = (x * tileScale.x - tileOffset.x);
			const float tileDrawPosY = (y * tileScale.y - tileOffset.y);
			const float tileDrawWidth = static_cast<float>(tileScale.x);
			const float tileDrawHeight = static_cast<float>(tileScale.y);

			for (auto & layer:tileMap->layers) 
			{
				
				const int tileID = layer.GetTileID(
					tilePosX
					, tilePosY
				);

				if (tileID == -1)
				{
					continue;
				}

				Tile* tile = tileMap->GetTile(tileID);
				if (tile == nullptr)
				{
					continue;
				}

				tileMap->GetTile(tileID)->spriteGPU->DrawSprite(
					tileDrawPosX
					, tileDrawPosY
					, tileDrawWidth
					, tileDrawHeight
					, WHITE
					, false
					, 0.0f
				);
			}
		}
	}

	//渲染物品到屏幕上
	for (const auto& instance : spriteRendererInstances)
	{

		if (instance.actor == nullptr)
		{
			continue;
		}

		const float objPosX = (instance.actor->transform.position.x - offset.x) * tileScale.x;
		const float objPosY = (instance.actor->transform.position.y - offset.y) * tileScale.y;

		instance.spriteGPU->DrawSprite(
			objPosX
			, objPosY
			, instance.scale.x * instance.sprite->w
			, instance.scale.y * instance.sprite->h
			, WHITE
			, instance.isFlip.x
			, (instance.actor == nullptr) ? 0.0f : instance.actor->transform.eulerAngle
		);


		for (int i = static_cast<int>(objPosX); i < static_cast<int>(objPosX + instance.scale.x * instance.sprite->w); i++)
		{
			for (int j = static_cast<int>(objPosY); j < static_cast<int>(objPosY + instance.scale.y * instance.sprite->h); j++)
			{
				const int index = static_cast<int>(j * Screen::width + i);

				if (index < 0 || index >= objectBufferWithRay.size())
				{
					continue;
				}
				objectBufferWithRay[index] = instance.actor;

			}
		}

	}
	spriteRendererInstances.clear();

}

noa::TileMapCamera& noa::TileMapCamera::SetTileMap(noa::TileMap* tileMap)
{
	this->tileMap = tileMap;
	return *this;
}

noa::TileMapCamera& noa::TileMapCamera::SetFollow(noa::Actor* actor)
{
	if (actor == nullptr)
	{
		return *this;
	}
	this->follow = &actor->transform;
	return *this;
}

noa::TileMapCamera* noa::TileMapCamera::Apply() {
	return this;
}


noa::Vector<float> noa::TileMapCamera::ScreenPointToWorld(float x, float y)
{
	Vector<float> result = { 0.0f,0.0f };
	result.x = offset.x + (x / tileScale.x) - 0.5f;
	result.y = offset.y + (y / tileScale.y) - 0.5f;
	return result;
}

noa::FreeCamera::FreeCamera(noa::Scene* scene) :Camera(scene)
{
	wallDistanceBuffer = std::vector<float>(Screen::width, 0.0);
	objectBufferWithRay = std::vector<NoaObject*>(Screen::width, nullptr);
}


noa::FreeCamera* noa::FreeCamera::Create(Scene* scene)
{
	return NObject<FreeCamera>::Create(scene);
}

void noa::FreeCamera::RenderFloor()
{

	//const float angle = follow->eulerAngle - halfFOV;
	//const float dirX = sinf(follow->eulerAngle);
	//const float dirY = cosf(follow->eulerAngle);
	//
	//const float eyeRayX = normalEyeRay * sinf(angle);
	//const float eyeRayY = normalEyeRay * cosf(angle);
	//const float planeX = -eyeRayX + dirX;
	//const float planeY = -eyeRayY + dirY;

	//const float rayDirX0 = eyeRayX;
	//const float rayDirY0 = eyeRayY;
	//const float rayDirX1 = dirX + planeX;
	//const float rayDirY1 = dirY + planeY;

	//for (int y = static_cast<int>(Screen::height * 0.5); y < Screen::height; y++)
	//{
	//	const float rowDistance = Screen::height/(2.0f*y - Screen::height);

	//	const float floorStepX = rowDistance * (2 * planeX) / Screen::width;
	//	const float floorStepY = rowDistance * (2 * planeY) / Screen::width;

	//	float floorX = follow->position.x + rowDistance * rayDirX0 + 0.5f;
	//	float floorY = follow->position.y + rowDistance * rayDirY0 + 0.5f;

	//	for (int x = 0; x < Screen::width; ++x)
	//	{
	//		const int cellX = (int)(floorX);
	//		const int cellY = (int)(floorY);

	//		const float simpleX = floorX - cellX;
	//		const float simpleY = floorY - cellY;

	//		floorX += floorStepX;
	//		floorY += floorStepY;

	//		const int floorTileID = map->GetTileID(cellX, cellY);
	//		const Tile* floorTile = map->GetTile(floorTileID);
	//		if (floorTileID == -1 || floorTile == nullptr)
	//		{
	//			renderer->DrawPixel(x, y, LIGHTRED);
	//			continue;
	//		}

	//		Uint32 color = floorTile->sprite->GetColor(simpleX, simpleY);

	//		//color = MULTICOLOR(color, multiColor);

	//		renderer->DrawPixel(x, y, color);

	//	}
	//}
}

void noa::FreeCamera::Render()
{
	if (follow == nullptr || map == nullptr)
	{
		return;
	}

	if (renderFloor)
	{
		RenderFloor();
	}

	for (int x = 0; x < Screen::width; x++)
	{
		Ray ray = std::move(RaycastHit(x));

		wallDistanceBuffer[x] = std::move(ray.distance);
		rayResult[x] = std::move(ray);

		const float ceiling = Screen::height * 0.5f - Screen::height / ray.distance;
		const float floor = Screen::height - ceiling;
		uint32_t color = ERRORCOLOR;

		for (int y = 0; y < Screen::height; y++)
		{
			if (y <= ceiling)
			{

				if (skybox == nullptr)
				{
					color = RGBA(63, 63, 63, 255);
					//color = MULTICOLOR(color, multiColor);
					renderer->DrawPixel(x, y, color);
					continue;
				}
				const float dx = (x + 200 * follow->eulerAngle) / Screen::width;
				const float dy = y / (Screen::height * 2.0f);

				color = skybox->GetColor(dy, dx);

				renderer->DrawPixel(x, y, color);

			}
			else if (y > ceiling && y <= floor)
			{
				ray.simple.y = (y - ceiling)
					/ (floor - ceiling);

				const Tile* tile = map->GetTile(ray.hitTile);
				if (tile != nullptr)
				{
					color = tile->sprite->GetColor(ray.simple.y, ray.simple.x);
				}

			}
			else if (!renderFloor)
			{
				color = RGBA(128, 128, 128, 255);

			}

			renderer->DrawPixel(x, y, color);

		}

	}

	RenderGameObject();
}

noa::Ray noa::FreeCamera::RaycastHit(int pixelX)
{
	Ray ray;
	ray.distance = 0.0f;
	ray.angle = follow->eulerAngle - FOV * (0.5f - (float)pixelX / Screen::width);
	const float rayForwordStep = 0.05f;
	const Vector<float>& eye = Vector<float>(sinf(ray.angle), cosf(ray.angle));
	bool isHitCollisionTile = map->IsCollisionTile(ray.hitTile);
	while (!isHitCollisionTile && ray.distance < viewDepth)
	{
		ray.distance += rayForwordStep;

		const Vector<float>& floatHitPoint = follow->position + eye * ray.distance + Vector<float>(0.5f, 0.5f);
		const Vector<int>& intHitPoint = Vector<int>(static_cast<int>(floatHitPoint.x), static_cast<int>(floatHitPoint.y));

		if (intHitPoint.x < 0 || intHitPoint.x >= static_cast<int>(map->w)
			|| intHitPoint.y < 0 || intHitPoint.y >= static_cast<int>(map->h))
		{
			ray.hitTile = -1;
			ray.distance = viewDepth;
			continue;
		}
		if (map->layers.empty())
		{
			continue;
		}
		ray.hitTile = map->layers[map->layers.size() - 1].GetTileID(intHitPoint.x, intHitPoint.y);
		ray.tilePosition = { intHitPoint.x,intHitPoint.y };

		isHitCollisionTile = map->IsCollisionTile(ray.hitTile);

		if (isHitCollisionTile)
		{

			const float blockMidX = intHitPoint.x + 0.5f;
			const float blockMidY = intHitPoint.y + 0.5f;

			const float testAngle = atan2f((floatHitPoint.y - blockMidY), (floatHitPoint.x - blockMidX));

			if (testAngle >= -PI * 0.25f && testAngle < PI * 0.25f)
			{
				ray.simple.x = floatHitPoint.y - intHitPoint.y;
			}
			else if (testAngle >= PI * 0.25f && testAngle < PI * 0.75f)
			{
				ray.simple.x = floatHitPoint.x - intHitPoint.x;
			}
			else if (testAngle < -PI * 0.25f && testAngle >= -PI * 0.75f)
			{
				ray.simple.x = floatHitPoint.x - intHitPoint.x;
			}
			else if (testAngle >= PI * 0.75f || testAngle < -PI * 0.75f)
			{
				ray.simple.x = floatHitPoint.y - intHitPoint.y;
			}
		}

	}

	ray.distance = ray.distance * cosf(follow->eulerAngle - ray.angle);

	return ray;
}

noa::FreeCamera& noa::FreeCamera::SetFollow(noa::Actor * actor) 
{
	if (actor == nullptr) 
	{
		return *this;
	}
	this->follow = &actor->transform;
	return *this;
}

noa::FreeCamera& noa::FreeCamera::SetTileMap(noa::TileMap* map)
{
	this->map = map;
	return *this;
}

noa::FreeCamera& noa::FreeCamera::SetSkybox(noa::Sprite* skybox)
{
	this->skybox = skybox;
	return *this;
}

noa::FreeCamera* noa::FreeCamera::Apply() {
	return this;
}

inline int Partition(std::vector<noa::SpriteRendererInstance>& arr, int low, int high) {
	const float pivot = arr[high].distanceToPlayer;
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++) {
		if (arr[j].distanceToPlayer >= pivot) {
			i++;
			std::swap(arr[i], arr[j]);
		}
	}
	std::swap(arr[i + 1], arr[high]);
	return (i + 1);
}

inline void QuickSort(std::vector<noa::SpriteRendererInstance>& arr, int low, int high)
{
	if (low >= high)
	{
		return;
	}

	const int pi = Partition(arr, low, high);
	QuickSort(arr, low, pi - 1);
	QuickSort(arr, pi + 1, high);

}

void noa::FreeCamera::RenderGameObjectEnter()
{

	if (spriteRendererInstances.empty())
	{
		return;
	}

	for (int i = 0; i < objectBufferWithRay.size(); i++)
	{
		objectBufferWithRay[i] = nullptr;
	}

	for (auto& instance : spriteRendererInstances)
	{
		if (instance.actor == nullptr)
		{
			continue;
		}
		instance.vecToPlayer = std::move(instance.actor->transform.position - follow->position);
		instance.distanceToPlayer = instance.vecToPlayer.Magnitude();

	}

	QuickSort(spriteRendererInstances, 0, static_cast<int>(spriteRendererInstances.size()) - 1);
}

void noa::FreeCamera::RenderGameObject()
{
	RenderGameObjectEnter();

	for (const auto& instance : spriteRendererInstances)
	{
		if (instance.actor == nullptr)
		{
			continue;
		}

		const Vector<float>& vecToFollow = instance.vecToPlayer;

		const float distanceFromPlayer = instance.distanceToPlayer;

		const Vector<float>& eye = Vector<float>(sinf(follow->eulerAngle), cosf(follow->eulerAngle));

		float objectAngle = atan2(eye.y, eye.x) - atan2(vecToFollow.y, vecToFollow.x);
		if (objectAngle <= -PI)
		{
			objectAngle += 2.0f * PI;
		}
		if (objectAngle > PI)
		{
			objectAngle -= 2.0f * PI;
		}


		const bool isInPlayerFOV = fabs(objectAngle) < HALFPI;

		if (isInPlayerFOV && distanceFromPlayer >= 0.5f &&
			distanceFromPlayer < viewDepth)
		{

			const float objectCeiling = Screen::height * 0.5f
				- Screen::height / distanceFromPlayer;

			const float objectFloor = Screen::height - objectCeiling;

			const float objectHeight = objectFloor - objectCeiling;
			const float objectWidth = objectHeight;

			const float middleOfObject = ((objectAngle / FOV + 0.5f))
				* Screen::width;

			const float objectPosZ = 2 * instance.actor->transform.posZ / distanceFromPlayer;

			for (float lx = 0; lx < objectWidth; lx++)
			{
				const int objectColumn = static_cast<int>(middleOfObject + lx - objectWidth * 0.5f);
				const float objSimpleX = lx / objectWidth;
				if (
					 objectColumn < 0 || objectColumn >= Screen::width
					|| wallDistanceBuffer[objectColumn] < distanceFromPlayer

					)
				{
					continue;
				}
				for (float ly = 0; ly < objectHeight; ly++)
				{
					const float objSimpleY = ly / objectHeight;
					Uint32 objColor = instance.sprite->GetColor(objSimpleX, objSimpleY);
					if (
						GetAValue(objColor) == 0
						|| (int)(objectCeiling + ly) < 0 || (int)(objectCeiling + ly) >= Screen::height
						)
					{
						continue;
					}
					//objColor = MULTICOLOR(objColor, multiColor);
					renderer->DrawPixel(objectColumn, (int)(objectCeiling + ly + objectPosZ), objColor);
					if (instance.actor->isRaycasted)
					{
						objectBufferWithRay[objectColumn] = instance.actor;
					}
					wallDistanceBuffer[objectColumn] = distanceFromPlayer;
					rayResult[objectColumn].hitTile = -1;

				}
			}
		}
	}

	spriteRendererInstances.clear();

}

noa::StaticCamera::StaticCamera(Scene* scene) :Camera(scene)
{

}

noa::StaticCamera* noa::StaticCamera::Create(Scene* scene)
{
	return NObject<StaticCamera>::Create(scene);
}

void noa::StaticCamera::SetBackground(noa::Sprite* sprite)
{
	if (sprite == nullptr)
	{
		return;
	}
	background = SpriteGPU::Create(sprite);
}

void noa::StaticCamera::SetTileScale(const Vector<int>& tileScale)
{
	this->tileScale = tileScale;
	visibleTiles = Vector<float>(
		static_cast<float>(Screen::width) / tileScale.x
		, static_cast<float>(Screen::height) / tileScale.y
	);
}

void noa::StaticCamera::Render()
{
	(background != nullptr) ? background->DrawSprite(0.0f, 0.0f, static_cast<float>(Screen::width), static_cast<float>(Screen::height), WHITE, false, 0.0f) : renderer->FullScreen(BLUE);

	for (const auto& instance : spriteRendererInstances)
	{
		//绘制Actor到屏幕位置
		if (instance.actor == nullptr)
		{
			continue;
		}

		const float posX = (instance.actor->transform.position.x) * tileScale.x;
		const float posY = (instance.actor->transform.position.y) * tileScale.y;
		instance.spriteGPU->DrawSprite(posX, posY, static_cast<float>(tileScale.x), static_cast<float>(tileScale.y), WHITE, false, 0.0f);
	}

	spriteRendererInstances.clear();

}


