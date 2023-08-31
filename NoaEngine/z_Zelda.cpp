#define Z_ZELDA
#ifdef Z_ZELDA

#include "./core/NoaEngine.h"

using namespace noa;

Vector<int> tileScale(64, 64);

class Player :public GameObject, public Rigidbody {
public:
	Player(TileMap* map) :
		GameObject(new Sprite(LoadSprFile("./Assets/Zelda/JumpMan.spr"),tileScale)),
		Rigidbody(&position)
	{

		useGravity = false;

		//设置地图的碰撞信息
		SetCollisionTileID({25,26,33,34,19,20,21,27,29,35,36,37});
		UpdateMap(map);
	}

	void ActorControl() {
		velocity.x = 0;
		velocity.y = 0;

		if (inputSystem.GetKeyHold(KeyA))
		{
			velocity.x = -speed;
		}

		if (inputSystem.GetKeyHold(KeyD))
		{
			velocity.x = speed;
		}

		if (inputSystem.GetKeyHold(KeyW))
		{
			velocity.y = -speed;
		}

		if (inputSystem.GetKeyHold(KeyS))
		{
			velocity.y = speed;
		}

	}

	void Start() override {

	}

	void Update() override {
		ActorControl();
	}
private:
	float speed = 8;
};

class ZeldaGame :public NoaGameEngine 
{
public:
	ZeldaGame(int width, int height, GameWindowMode windowMode, string gameName) :
		NoaGameEngine(width,height,windowMode,gameName) 
	{
		text.position = Vector<int>(10, 10);
		text.textColor = WHITE;

		button.sprite = new Sprite(LoadSprFile("./Assets/Zelda/JumpMan.spr"), Vector<int>(1, 1));
		button.position = Vector<int>(pixelWidth / 2, pixelHeight / 2);
		button.AddClickEvent(
			[this]()
			{
				canvase.SetActive(false);
			});

		canvase.uiComponent.push_back(&button);
		canvase.uiComponent.push_back(&text);

	}

	void Start() override 
	{

	}

	void Update() override 
	{
		Vector<int> drawPos =  camera.Render(tileMap,frontDelta,endDelta);
		player.sprite->DrawSprite(drawPos.x, drawPos.y, true);

		text.text = move("FPS:" + to_string(1 / deltaTime));

		if (inputSystem.GetMouseButton(RightButton))
		{
			canvase.SetActive(true);
		}

		//renderer.DrawString(("FPS:"+to_string(1/deltaTime)), 10, 10, WHITE, 30);

	}

private:
	TileMap tileMap = TileMap(
		{
			LoadTileFromTsd("./Assets/Zelda/map/tileSet.tsd"),
			LoadMapFromCSV("./Assets/Zelda/map/level.csv")
		}
	);

	Player player = Player(&tileMap);
	TileMapCamera camera = TileMapCamera(tileScale, &player.position);
	Vector<float> frontDelta = Vector<float>(0.0, 0.0);
	Vector<float> endDelta = Vector<float>(0.0, -0.5);

	/*NoaButton button1;*/

	//ui
	NoaCanvase canvase;

	NoaText text;
	NoaButton button;

};

int main(int argc,char * argv)
{
	ZeldaGame game(1920/2,1080/2,NoaGameEngine::WindowMode,"Zelda");
	game.Run();
	return 0;
}

#endif // Z_ZELDA