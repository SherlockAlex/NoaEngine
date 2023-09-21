#include "Player.h"
#include "Enimy.h"
#include "M4A1.h"

Player::Player(Scene* scene) :Actor(scene)
{
	rigid->collision.sacle = {-0.2,-0.2};

	rigid->tag = "Player";
	rigid->useGravity = false;

	rigid->damping = 0;

	inputSystem.inputEvent += [this]() {
		this->RotateControl();
		};

	MakeGun();

}

Player* Player::Create(Scene* scene)
{
	return new Player(scene);
}

Player::~Player() {
	Actor::~Actor();
	//Rigidbody::~Rigidbody();
}

void Player::SetPosition(int tileID, MapFile& tileMap)
{
	for (int i = 0; i < tileMap.w; i++)
	{
		for (int j = 0; j < tileMap.h; j++)
		{
			if (tileMap.image[j * tileMap.w + i] == tileID)
			{
				transform.position.x = i;
				transform.position.y = j;
			}
		}
	}

	

}

static float walkSpeed = 0;
void Player::ActorControl() 
{

	rigid->velocity.x = 0;
	rigid->velocity.y = 0;

	if (!inputSystem.GetKeyHold(KeyM)) 
	{
		if (inputSystem.GetKeyHold(KeyW))
		{
			rigid->velocity.x += sinf(transform.eulerAngle);
			rigid->velocity.y += cosf(transform.eulerAngle);
		}

		if (inputSystem.GetKeyHold(KeyS))
		{
			rigid->velocity.x += -sinf(transform.eulerAngle);
			rigid->velocity.y += -cosf(transform.eulerAngle);
		}

		if (inputSystem.GetKeyHold(KeyA))
		{
			rigid->velocity.x += -cosf(transform.eulerAngle);
			rigid->velocity.y += sinf(transform.eulerAngle);
		}

		if (inputSystem.GetKeyHold(KeyD))
		{
			rigid->velocity.x += cosf(transform.eulerAngle);
			rigid->velocity.y += -sinf(transform.eulerAngle);
		}
	}
	else 
	{
		if (inputSystem.GetKeyHold(KeyW))
		{
			rigid->velocity.x += 0;
			rigid->velocity.y += -1;
		}

		if (inputSystem.GetKeyHold(KeyS))
		{
			rigid->velocity.x += 0;
			rigid->velocity.y += 1;
		}

		if (inputSystem.GetKeyHold(KeyA))
		{
			rigid->velocity.x += -1;
			rigid->velocity.y += 0;
		}

		if (inputSystem.GetKeyHold(KeyD))
		{
			rigid->velocity.x += 1;
			rigid->velocity.y += 0;
		}
	}

	if (inputSystem.GetMouseButton(LeftButton)&&bulletCount>0)
	{
		guns[currentGunIndex]->Shoot();
	}

	if (inputSystem.GetKeyDown(KeyE))
	{
		const Ray ray = camera->GetRayInfo(pixelWidth * 0.5);
		const float distanceToWall = ray.distance;
		if (distanceToWall < 1.5)
		{
			if (ray.hitTile == 98) 
			{
				this->rigid->GetTileMap()->SetTileID(ray.tilePosition.x,ray.tilePosition.y,107);
				interactAFX.Play(false);
			}
			else if (ray.hitTile == 102&&sceneManager.GetActiveScene()->name == "NewGame")
			{
				sceneManager.LoadScene("SecondFloor");
			}
			//Debug(to_string(camera->GetRayInfo(pixelWidth * 0.5).hitTile));
		}
		
	}
	
	rigid->velocity = rigid->velocity.Normalize() * speed;

}

void Player::RotateControl()
{
	if (inputSystem.GetMouseMoveState())
	{
		Vector<float> mouseDelta = inputSystem.GetMouseMoveDelta();
		transform.eulerAngle += mouseSpeed * mouseDelta.x * deltaTime;
	}

	int deltaIndex = inputSystem.GetMouseWheel().y;

	if (deltaIndex<0)
	{
		currentGunIndex--;
		if (currentGunIndex<0) 
		{
			currentGunIndex = guns.size() - 1;
		}
	}

	else if (deltaIndex > 0) {
		currentGunIndex++;
		if (currentGunIndex >= guns.size())
		{
			currentGunIndex = 0;
		}
	}

}

void Player::Start()
{
	

}

void Player::Update()
{
	ActorControl();

	float vel = rigid->velocity.SqrMagnitude();

	if (!inputSystem.GetKeyHold(KeyM)) 
	{
		guns[currentGunIndex]->Update();
		//shotgun->Update();
	}

	renderer.DrawString("hp:"+to_string(this->hp)+"\nbullet:" + to_string(bulletCount), 0, 0, RED, pixelHeight / 20);

}

void Player::TakeDamage(int damage)
{
	LiveEntity::TakeDamage(damage);
	painAFX.Play(false);
}

void Player::MakeGun()
{
	shotgun = new Shotgun(&bulletCount, this->camera);
	shotgun->animation->SetActiveScene(this->activeScene);
	shotgun->damage = 110;
	shotgun->takeBullet = 7;

	pistol = new Pistol(&bulletCount, this->camera);
	pistol->animation->SetActiveScene(this->activeScene);
	pistol->damage = 18;
	pistol->takeBullet = 0;

	m4a1 = new M4A1(&bulletCount, this->camera);
	m4a1->animation->SetActiveScene(this->activeScene);
	m4a1->damage = 32;
	m4a1->takeBullet = 1;

	guns.push_back(pistol);
	guns.push_back(shotgun);
	guns.push_back(m4a1);

}

void Player::SetCamera(FreeCamera* camera)
{
	this->camera = camera;
	shotgun->camera = camera;
	pistol->camera = camera;
	m4a1->camera = camera;
}
