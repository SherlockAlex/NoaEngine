#include "NoaEngine.h"

using namespace noa;

class Player:public GameObject,public Rigidbody
{
public:
	Player() :GameObject(new Sprite(LoadSprFile("./Assets/JumpMan/JumpMan.spr"), Vector<float>(0.3,0.3))), Rigidbody(&position)
	{
		//玩家的构造函数
		colliderSize.x = 0.45;
		colliderSize.y = 0.55;

		position = Vector<float>(0.0, 0.0);

		//useGravity = false;

	}
	~Player()
	{
		GameObject::~GameObject();
		Rigidbody::~Rigidbody();
	}

	void Start() override 
	{

	}
	Vector<float> jumpForce = Vector<float>(0.0, -15.0);
	void Update() override 
	{
		velocity.x = 0;
		//if (IsFocused()) 
		{

			if (inputSystem.GetKeyHold(KeyA))
			{
				velocity.x = -speed;

			}
			if (inputSystem.GetKeyHold(KeyD))
			{
				velocity.x = speed;

			}

			if (inputSystem.GetKeyHold(KeyK) && isGrounded)
			{
				AddForce(jumpForce, Impulse);
				jumpSFX.Play(false);
			}

		}
	}

public:
	float speed = 10;
	const Audio coinSFX = Audio("./Assets/JumpMan/Music/coin.mp3", Chunk);
	const Audio jumpSFX = Audio("./Assets/JumpMan/Music/jump.mp3", Chunk);
};

class Platformer :public NoaGameEngine {
public:
	Platformer(int width, int height, GameWindowMode windowMode, string gameName) :NoaGameEngine(width, height, windowMode, gameName) 
	{
		player.UpdateMap(&tileMap);
		player.SetCollisionTileID({ 0 });
		BGM.Play(true);

		for (int i = 0; i < tileMap.w; i++)
		{
			for (int j = 0; j < tileMap.h; j++)
			{

				if (tileMap.GetTileID(i,j) == 61)
				{
					player.position.x = i;
					player.position.y = j;
					player.velocity.y = 0;
				}

			}
		}
	}

	void Start() override
	{

	}

	void Update() override 
	{

		cameraPosition = player.position;

		//Draw level
		
		Vector<float> visibleTiles(int(pixelWidth / tileScale.x), int(pixelHeight / tileScale.y));

		//calculate Top-Leftmost visible tile
		Vector<float> offset = cameraPosition - visibleTiles * 0.5;

		//Clamp camera to game boundaries
		if (offset.x < 0)
		{
			offset.x = 0;
		}
		if (offset.y < 0)
		{
			offset.y = 0;
		}
		if (offset.x>tileMap.w - visibleTiles.x)
		{
			offset.x = tileMap.w - visibleTiles.x;
		}
		if (offset.y > tileMap.h - visibleTiles.y)
		{
			offset.y = tileMap.h - visibleTiles.y;
		}
		
		//Get offsets for smooth movement
		Vector<float> tileOffset;
		tileOffset.x = (offset.x - (int)offset.x) * tileScale.x;
		tileOffset.y = (offset.y - (int)offset.y) * tileScale.y;

		for (int x = -2;x<visibleTiles.x+2;x++) 
		{
			for (int y = -2; y < visibleTiles.y + 2; y++)
			{
				//首先获取tile
				const int tileID = tileMap.GetTileID(x + offset.x, y + offset.y);
				if (tileID == -1)
				{
					renderer.DrawRect(Vector<int>(x * tileScale.x - tileOffset.x, y * tileScale.y - tileOffset.y), Vector<int>((x + 1) * tileScale.x - tileOffset.x, (y + 1) * tileScale.y - tileOffset.y), BLUE);
					continue;
				}
				//renderer.DrawRect(Vector<int>(x * tileScale.x - tileOffset.x, y * tileScale.y - tileOffset.y), Vector<int>((x + 1) * tileScale.x - tileOffset.x, (y + 1) * tileScale.y - tileOffset.y), RED);
				renderer.DrawRect(Vector<int>(x * tileScale.x - tileOffset.x, y * tileScale.y - tileOffset.y), Vector<int>((x + 1) * tileScale.x - tileOffset.x, (y + 1) * tileScale.y - tileOffset.y), *tileMap.GetTile(tileID)->sprite);
				
			}
		}

		//Draw player
		renderer.DrawRect(
			Vector<int>((player.position.x - offset.x) * tileScale.x, (player.position.y - offset.y) * tileScale.y),
			Vector<int>((player.position.x - offset.x+1) * tileScale.x, (player.position.y - offset.y+1) * tileScale.y),
			*player.sprite);
		//player.sprite->DrawSprite((player.position.x - offset.x) * tileScale.x, (player.position.y - offset.y)*tileScale.y, true);


	}

private:
	//0 - wall
	//1 - coin
	//other - background
	TileMap tileMap = TileMap(
		LoadTileFromTsd("./Assets/JumpMan/Tile/tileSet.tsd"),
		LoadMapFromCSV("./Assets/JumpMan/Map/level1.csv")
	);

	Vector<float> cameraPosition = Vector<float>(0.0,0.0);
	Vector<int> tileScale = Vector<int>(64, 64);

	Player player;

	Audio BGM = Audio("./Assets/JumpMan/Music/BGM.ogg", Music);
	Audio gameOverMusic = Audio("./Assets/JumpMan/Music/gameover.mp3", Chunk);

};

int main(int argc,char * argv[])
{
	Platformer game(1920 / 2, 1080 / 2, NoaGameEngine::WindowMode, "SuperMario");
	game.Run();
	return 0;
}