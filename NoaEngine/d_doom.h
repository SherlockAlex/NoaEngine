//#define DOOM
#ifdef DOOM

#ifndef D_DOOM_H
#define D_DOOM_H

#include "NoaEngine.h"
#include <map>

extern void GameStart(void);
extern void GameUpdate(void);

extern vector<GameObject*> gameObjects;

class Doom :public NoaGameEngine {


private:

public:
	Doom(int width, int height,
		GameWindowMode windowMode,
		string gameName) :NoaGameEngine(width, height, windowMode, gameName)
	{
		//game inintialize
	}

public:

	void Start() override
	{
		//called once when game start
		GameStart();
	}

	void Update() override
	{
		//called when each frame
		GameUpdate();
	}


};

extern Doom game;

class Player {
public:
	Vector<float> position;
	float angle = 0.0f;
	float FOV = PI * 0.25f;
	float viewDepth = 30.0f;

	float speed = 5.0f;

public:
	Player()
	{
		position.x = 15.0f;
		position.y = 5.9f;
	}
};

typedef struct Ray {
	//返回射线碰撞到的信息
	float angle = 0.0f;
	float distance = 0.0f;
	bool isHitDoor = false;
	bool isHitWall = false;

	//返回贴图信息
	Vector<float> simple;

}Ray;

extern Ray RayCastHit(
	int pixelX,			//像素点横坐标
	Player& player,		//玩家对象引用
	LevelMap& map		//当前关卡地图引用
);


class Bullet :public Behaviour {
	//子弹类
public:
	Vector<float> direction;
	float speed = 0;
	int damage = 50;
	LevelMap* map = nullptr;

public:
	Bullet() :Behaviour()
	{

	}

	Bullet(Vector<float> startPos, Vector<float> direction, int damage, float speed, LevelMap* map) :Behaviour()
	{
		position = startPos;
		this->direction = direction;
		this->speed = speed;
		this->map = map;
		this->damage = damage;

	}

	void Start() override
	{

	}

	void Update() override
	{
		position.x = position.x + direction.x * speed * game.DeltaTime();
		position.y = position.y + direction.y * speed * game.DeltaTime();

		//cout << "[info]:子弹坐标,x = " << position.x << ",y = " << position.y << endl;

		if ((int)position.x < 0 || (int)position.x >= map->w || (int)position.y < 0 || (int)position.y >= map->h)
		{
			//cout << "[info]:子弹越界,x = " << position.x << ",y = " << position.y << endl;
			//Destroy();
			Destroy(this);
		}

		Uint8  hitObject = map->level[(int)position.y * map->w + (int)position.x];

		if (hitObject == 127
			|| hitObject == 0)
		{
			//命中墙壁

			//cout << "[info]:子弹命中墙壁,x = " << position.x << ",y = " << position.y << endl;
			Destroy(this);
		}

		for (int i = 0; i < gameObjects.size(); i++)
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
			if (distance < 1)
			{
				vector<void*> other;
				other.push_back(&damage);
				gameObjects[i]->OnTriggerEnter(other);
				Destroy(this);
			}
		}

	}
};

class Enimy :public GameObject
{

private:
	int hp = 200;

	LevelMap* map;

public:
	Enimy(Sprite enimySprite, Vector<float> startPosition, LevelMap* map) :GameObject(enimySprite, startPosition) {
		this->map = map;
		isTrigger = true;
	}

	void Start() override {

	}

	void Update() override {

	}

	void TakeDamage(int damage)
	{
		Debug("角色被攻击");
		//cout << "[info]:角色被攻击" << endl;
		hp -= damage;
		if (hp <= 0)
		{
			Debug("角色死亡");
			Destroy(this);
		}
	}

	void OnTriggerEnter(vector<void*> other) override {
		if (other.size() <= 0) {
			return;
		}

		TakeDamage(*(int*)other[0]);

	}

};

typedef struct GunFile {
	int deltax;			//枪支的屏幕坐标偏移量
	int deltay;
	int damage;			//子弹威力

	//第一帧大小
	int w;
	int h;

	AnimatorFile amt;	//枪支动画
};

//用于加载本地的gun文件
inline GunFile LoadGunFromFile(const char* file) {
	GunFile gunFile;

	try {
		// 创建二进制读取器
		std::ifstream reader(file, std::ios::binary);

		// 读取枪支属性
		reader.read(reinterpret_cast<char*>(&gunFile.deltax), sizeof(gunFile.deltax));
		reader.read(reinterpret_cast<char*>(&gunFile.deltay), sizeof(gunFile.deltay));
		reader.read(reinterpret_cast<char*>(&gunFile.damage), sizeof(gunFile.damage));
		reader.read(reinterpret_cast<char*>(&gunFile.w), sizeof(gunFile.w));
		reader.read(reinterpret_cast<char*>(&gunFile.h), sizeof(gunFile.h));

		// 读取枪支动画属性
		reader.read(reinterpret_cast<char*>(&gunFile.amt.posx), sizeof(gunFile.amt.posx));
		reader.read(reinterpret_cast<char*>(&gunFile.amt.posy), sizeof(gunFile.amt.posy));
		reader.read(reinterpret_cast<char*>(&gunFile.amt.w), sizeof(gunFile.amt.w));
		reader.read(reinterpret_cast<char*>(&gunFile.amt.h), sizeof(gunFile.amt.h));

		// 读取枪支动画数据
		for (int i = 0; i < gunFile.amt.h; i++) {
			uint32_t* frameData = new uint32_t[gunFile.amt.w];
			reader.read(reinterpret_cast<char*>(frameData), sizeof(uint32_t) * gunFile.amt.w);
			gunFile.amt.data.push_back(frameData);
		}

		reader.close();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "读取文件失败: " << e.what() << std::endl;
	}

	return gunFile;
}

class Gun {
	//游戏枪支类
public:
	int damage = 200;
private:
	//枪支再屏幕的位置
	int x = 0;
	int y = 0;

	//deltaX一般有GunFile给出
	int deltaX = 0;
	int deltaY = 0;

	//枪支和动画
	Sprite* sprite;
	Animator* animator = nullptr;

public:
	Gun() {

	}

	Gun(Sprite* sprite, Animator* animator)
	{
		this->sprite = sprite;
		this->animator = animator;

		x = pixelWidth / 2;
		y = pixelHeight - (int)((sprite->h / sprite->w) * (pixelWidth / 4) * 0.5f);
	}

	//从本地的文件中加载枪支
	Gun(const char* filename)
	{
		GunFile gunFile = LoadGunFromFile(filename);
		animator = new Animator(gunFile.amt);
		sprite = new Sprite(gunFile.w, gunFile.h, 4, animator->GetCurrentFrameImage());

		x = pixelWidth / 2;
		y = pixelHeight - (int)((sprite->h / sprite->w) * (pixelWidth / 4) * 0.5f);

		this->deltaX = gunFile.deltax;
		this->deltaY = gunFile.deltay;
	}

	void RenderGun(Vector<int> offset) {
		//渲染枪支到屏幕上
		sprite->DrawSprite(x + offset.x + deltaX, y + offset.y + deltaY, true);
		sprite->UpdateImage(animator->GetCurrentFrameImage());
	}

	void Shoot()
	{
		//射击按钮按下
		animator->Play();
		
	}

};


#endif
#endif // DOOM