#include "GameEngine.h"
#include "Player.h"
#include "Enimy.h"
#include "Item.h"
#include "Caco.h"
using namespace noa;

class Wolf3D :public NoaGameEngine
{
public:
	Wolf3D(int width, int height, NoaGameEngine::GameWindowMode windowMode, string gameName) :
		NoaGameEngine(width, height, windowMode, gameName)
	{
		player.SetPosition(35, objectMap);
		player.camera = &camera;
		//bulletAnimator.LoadFromAnimatorFile("./Assets/Wolf/bullet.amt");
		//bulletSprite = new Sprite(bulletAnimator.GetCurrentFrameImage(),Vector<int>(64,64));


		for (int i = 0; i < objectMap.w; i++)
		{
			for (int j = 0; j < objectMap.h; j++)
			{
				if (objectMap.image[j * objectMap.w + i] == 18)
				{
					Caco* enimy = new Caco(new Sprite(cacoSprite), &player.transform,&player);
					enimy->UpdateMap(&tileMap);
					enimy->transform.position.x = i;
					enimy->transform.position.y = j;
					enimy->transform.posZ = 0;
				}
			}
		}

		for (int i = 0; i < objectMap.w; i++)
		{
			for (int j = 0; j < objectMap.h; j++)
			{
				if (objectMap.image[j * objectMap.w + i] == 28)
				{
					Item* bullet = new Item(&bulletSprite);
					bullet->UpdateMap(&tileMap);
					bullet->transform.position.x = i;
					bullet->transform.position.y = j;

					bullet->pickEvent += [this]() {
						player.bulletCount += 5;
						bulletPickUpSFX.Play(false);
						};

					

				}
			}
		}

		
		vector<int> collisionTileID;
		collisionTileID.push_back(36);
		for (int i = 0; i < 108; i++)
		{
			if (i == 107)
			{
				continue;
			}

			collisionTileID.push_back(i);

		}
		tileMap.SetCollisionTileID(collisionTileID);


	}

	void Start() override
	{
		BGM.Play(true);
		inputSystem.SetRelativeMouseMode(true);
	}

	float i = 0;
	uint32_t mutiColor = WHITE;
	uint32_t darkLight = RGB(20, 20, 20);
	void Update() override {

		if (inputSystem.GetKeyDown(KeyK))
		{
			if(mutiColor == WHITE)
			{
				mutiColor = darkLight;
			}
			else {
				mutiColor = WHITE;
			}
		}
		
		camera.Render(tileMap, false, nullptr, mutiColor);
		mouse.DrawSprite(pixelWidth * 0.5 - 0.5 * mouse.scale.x, pixelHeight * 0.5 - 0.5 * mouse.scale.y, true);
		if (inputSystem.GetKeyHold(KeyM))
		{
			Vector<int> drawPos = mapCamera.Render(tileMap, Vector<float>(0.0, 0.0), Vector<float>(0.0, 0.0));
			renderer.DrawRect(drawPos, drawPos + Vector<int>(32, 32), WHITE);
		}

		if (inputSystem.GetKeyHold(KeyESC))
		{
			Quit();
		}

	}

private:

	Sprite sky = Sprite(resource.LoadSprFile("./Assets/Wolf/sky-sun.spr"), Vector<int>(1.0, 1.0));
	TileMap tileMap = TileMap(
		resource.LoadTileFromTsd("./Assets/Wolf/Map/tileSet.tsd"),
		resource.LoadMapFromCSV("./Assets/Wolf/Map/level_µÿÕº≤„.csv")
	);

	Player player = Player(&tileMap);
	FreeCamera camera = FreeCamera(&player.transform);
	TileMapCamera mapCamera = TileMapCamera(Vector<int>(32, 32), &player.transform);
	Sprite mouse = Sprite(resource.LoadSprFile("./Assets/Wolf/mouse.spr"), Vector<int>(0.03 * pixelWidth, 0.03 * pixelWidth));

	Sprite cacoSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/caco.spr"), Vector<int>(32, 32));

	//Animator bulletAnimator = Animator(7);
	Sprite bulletSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/bullet.spr"), Vector<int>(64, 64));

	Audio bulletPickUpSFX = Audio("./Assets/Wolf/Music/pickUpBullet.mp3", Chunk);

	Audio BGM = Audio("./Assets/Wolf/Music/theme.mp3", Music);

	MapFile objectMap = resource.LoadMapFromCSV("./Assets/Wolf/Map/level_∂‘œÛ.csv");

};

int main(int argc,char * argv[])
{
	Wolf3D wolf3D(1920 / 2, 1080 / 2, NoaGameEngine::WindowMode, "3D");
	wolf3D.Run();
	return 0;
}