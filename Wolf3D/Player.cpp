#include "Player.h"
#include "Enimy.h"


Player::Player(TileMap* map) :Behaviour(), Rigidbody(&transform)
{
	collision.sacle = {-0.2,-0.2};

	tag = "Player";
	useGravity = false;

	damping = 0;

	UpdateMap(map);

	inputSystem.inputEvent += [this]() {
		this->RotateControl();
		};

	MakeGun();

}

Player::~Player() {
	Behaviour::~Behaviour();
	Rigidbody::~Rigidbody();
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
void Player::ActorControl() {

	velocity.x = 0;
	velocity.y = 0;

	if (!inputSystem.GetKeyHold(KeyM)) 
	{
		if (inputSystem.GetKeyHold(KeyW))
		{
			velocity.x += sinf(transform.eulerAngle);
			velocity.y += cosf(transform.eulerAngle);
		}

		if (inputSystem.GetKeyHold(KeyS))
		{
			velocity.x += -sinf(transform.eulerAngle);
			velocity.y += -cosf(transform.eulerAngle);
		}

		if (inputSystem.GetKeyHold(KeyA))
		{
			velocity.x += -cosf(transform.eulerAngle);
			velocity.y += sinf(transform.eulerAngle);
		}

		if (inputSystem.GetKeyHold(KeyD))
		{
			velocity.x += cosf(transform.eulerAngle);
			velocity.y += -sinf(transform.eulerAngle);
		}
	}
	else 
	{
		if (inputSystem.GetKeyHold(KeyW))
		{
			velocity.x += 0;
			velocity.y += -1;
		}

		if (inputSystem.GetKeyHold(KeyS))
		{
			velocity.x += 0;
			velocity.y += 1;
		}

		if (inputSystem.GetKeyHold(KeyA))
		{
			velocity.x += -1;
			velocity.y += 0;
		}

		if (inputSystem.GetKeyHold(KeyD))
		{
			velocity.x += 1;
			velocity.y += 0;
		}
	}

	if (inputSystem.GetMouseButton(LeftButton)&&bulletCount>0)
	{
		guns[currentGunIndex]->Shoot();
		//shotgun->Shoot();
	}

	
	velocity = velocity.Normalize() * speed;

}

void Player::RotateControl()
{
	if (inputSystem.GetMouseMoveState())
	{
		Vector<float> mouseDelta = inputSystem.GetMouseMoveDelta();
		transform.eulerAngle += 0.025 * mouseDelta.x * deltaTime;
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

	float vel = velocity.SqrMagnitude();

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
	shotgun->damage = 110;
	shotgun->takeBullet = 7;

	pistol = new Pistol(&bulletCount, this->camera);
	pistol->damage = 10;
	pistol->takeBullet = 1;

	guns.push_back(pistol);
	guns.push_back(shotgun);

}

void Player::SetCamera(FreeCamera* camera)
{
	this->camera = camera;
	shotgun->camera = camera;
	pistol->camera = camera;
}
