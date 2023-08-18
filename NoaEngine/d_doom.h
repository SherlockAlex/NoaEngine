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
	//����������ײ������Ϣ
	float angle = 0.0f;
	float distance = 0.0f;
	bool isHitDoor = false;
	bool isHitWall = false;

	//������ͼ��Ϣ
	Vector<float> simple;

}Ray;

extern Ray RayCastHit(
	int pixelX,			//���ص������
	Player& player,		//��Ҷ�������
	LevelMap& map		//��ǰ�ؿ���ͼ����
);


class Bullet :public Behaviour {
	//�ӵ���
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

		//cout << "[info]:�ӵ�����,x = " << position.x << ",y = " << position.y << endl;

		if ((int)position.x < 0 || (int)position.x >= map->w || (int)position.y < 0 || (int)position.y >= map->h)
		{
			//cout << "[info]:�ӵ�Խ��,x = " << position.x << ",y = " << position.y << endl;
			//Destroy();
			Destroy(this);
		}

		Uint8  hitObject = map->level[(int)position.y * map->w + (int)position.x];

		if (hitObject == 127
			|| hitObject == 0)
		{
			//����ǽ��

			//cout << "[info]:�ӵ�����ǽ��,x = " << position.x << ",y = " << position.y << endl;
			Destroy(this);
		}

		for (int i = 0; i < gameObjects.size(); i++)
		{
			if (!gameObjects[i]->isTrigger)
			{
				continue;
			}

			//�����ɫ���ӵ�֮��ľ���

			float deltaX = gameObjects[i]->position.x - position.x;
			float deltaY = gameObjects[i]->position.y - position.y;

			float distance = deltaX * deltaX + deltaY * deltaY;

			//ִ���¼�
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
		Debug("��ɫ������");
		//cout << "[info]:��ɫ������" << endl;
		hp -= damage;
		if (hp <= 0)
		{
			Debug("��ɫ����");
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
	int deltax;			//ǹ֧����Ļ����ƫ����
	int deltay;
	int damage;			//�ӵ�����

	//��һ֡��С
	int w;
	int h;

	AnimatorFile amt;	//ǹ֧����
};

//���ڼ��ر��ص�gun�ļ�
inline GunFile LoadGunFromFile(const char* file) {
	GunFile gunFile;

	try {
		// ���������ƶ�ȡ��
		std::ifstream reader(file, std::ios::binary);

		// ��ȡǹ֧����
		reader.read(reinterpret_cast<char*>(&gunFile.deltax), sizeof(gunFile.deltax));
		reader.read(reinterpret_cast<char*>(&gunFile.deltay), sizeof(gunFile.deltay));
		reader.read(reinterpret_cast<char*>(&gunFile.damage), sizeof(gunFile.damage));
		reader.read(reinterpret_cast<char*>(&gunFile.w), sizeof(gunFile.w));
		reader.read(reinterpret_cast<char*>(&gunFile.h), sizeof(gunFile.h));

		// ��ȡǹ֧��������
		reader.read(reinterpret_cast<char*>(&gunFile.amt.posx), sizeof(gunFile.amt.posx));
		reader.read(reinterpret_cast<char*>(&gunFile.amt.posy), sizeof(gunFile.amt.posy));
		reader.read(reinterpret_cast<char*>(&gunFile.amt.w), sizeof(gunFile.amt.w));
		reader.read(reinterpret_cast<char*>(&gunFile.amt.h), sizeof(gunFile.amt.h));

		// ��ȡǹ֧��������
		for (int i = 0; i < gunFile.amt.h; i++) {
			uint32_t* frameData = new uint32_t[gunFile.amt.w];
			reader.read(reinterpret_cast<char*>(frameData), sizeof(uint32_t) * gunFile.amt.w);
			gunFile.amt.data.push_back(frameData);
		}

		reader.close();
	}
	catch (std::ifstream::failure e) {
		std::cerr << "��ȡ�ļ�ʧ��: " << e.what() << std::endl;
	}

	return gunFile;
}

class Gun {
	//��Ϸǹ֧��
public:
	int damage = 200;
private:
	//ǹ֧����Ļ��λ��
	int x = 0;
	int y = 0;

	//deltaXһ����GunFile����
	int deltaX = 0;
	int deltaY = 0;

	//ǹ֧�Ͷ���
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

	//�ӱ��ص��ļ��м���ǹ֧
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
		//��Ⱦǹ֧����Ļ��
		sprite->DrawSprite(x + offset.x + deltaX, y + offset.y + deltaY, true);
		sprite->UpdateImage(animator->GetCurrentFrameImage());
	}

	void Shoot()
	{
		//�����ť����
		animator->Play();
		
	}

};


#endif
#endif // DOOM