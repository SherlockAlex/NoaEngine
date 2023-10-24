#include "Player.h"
#include "Enimy.h"
#include "M4A1.h"
#include "Enter.h"

Player::Player(Scene* scene) :Actor(scene)
{
	rigid->SetTileColliderScale(0.8,0.8);

	tag = "Player";
	rigid->useGravity = false;

	rigid->damping = 0;

	MakeGun();

}

Player* Player::Create(Scene* scene)
{
	return new Player(scene);
}

Player::~Player() {
	if (!guns.empty()) 
	{
		for (int i = 0; i < guns.size(); i++)
		{
			if (guns[i] != nullptr)
			{
				guns[i]->Delete();
			}
		}
	}
	
	
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

	this->RotateControl();

	if (!inputSystem.GetKeyHold(KeyCode::KEY_M)) 
	{
		if (inputSystem.GetKeyHold(KeyCode::KEY_W))
		{
			rigid->velocity.x += sinf(transform.eulerAngle);
			rigid->velocity.y += cosf(transform.eulerAngle);
		}

		if (inputSystem.GetKeyHold(KeyCode::KEY_S))
		{
			rigid->velocity.x += -sinf(transform.eulerAngle);
			rigid->velocity.y += -cosf(transform.eulerAngle);
		}

		if (inputSystem.GetKeyHold(KeyCode::KEY_A))
		{
			rigid->velocity.x += -cosf(transform.eulerAngle);
			rigid->velocity.y += sinf(transform.eulerAngle);
		}

		if (inputSystem.GetKeyHold(KeyCode::KEY_D))
		{
			rigid->velocity.x += cosf(transform.eulerAngle);
			rigid->velocity.y += -sinf(transform.eulerAngle);
		}
	}
	else 
	{
		if (inputSystem.GetKeyHold(KeyCode::KEY_W))
		{
			rigid->velocity.x += 0;
			rigid->velocity.y += -1;
		}

		if (inputSystem.GetKeyHold(KeyCode::KEY_S))
		{
			rigid->velocity.x += 0;
			rigid->velocity.y += 1;
		}

		if (inputSystem.GetKeyHold(KeyCode::KEY_A))
		{
			rigid->velocity.x += -1;
			rigid->velocity.y += 0;
		}

		if (inputSystem.GetKeyHold(KeyCode::KEY_D))
		{
			rigid->velocity.x += 1;
			rigid->velocity.y += 0;
		}
	}

	if (inputSystem.GetMouseButton(MouseButton::LEFT_BUTTON)&&bulletCount>0)
	{
		guns[currentGunIndex]->Shoot();
	}

	if (inputSystem.GetKeyDown(KeyCode::KEY_E))
	{
		const Ray ray = camera->GetRayInfo(Screen::width * 0.5);
		const float distanceToWall = ray.distance;
		if (distanceToWall < 0.5)
		{
			Enter* enter = nullptr;

			switch (ray.hitTile)
			{
			case 98:
				this->rigid->GetTileMap()->SetTileID(ray.tilePosition.x, ray.tilePosition.y, 107);
				interactAFX.Play(false);
				break;
			case 102:
				//执行对应的事件
				//去搜索这个位置上的trigger
				enter = sceneManager.FindActorWithType<Enter>();
				if (enter!=nullptr)
				{
					enter->Load();
				}
				break;
			default:
				break;
			}

			/*if (ray.hitTile == 98) 
			{
				this->rigid->GetTileMap()->SetTileID(ray.tilePosition.x,ray.tilePosition.y,107);
				interactAFX.Play(false);
			}

			else if (ray.hitTile == 102&&sceneManager.GetActiveScene()->GetName() == "NewGame")
			{
				sceneManager.LoadScene("SecondFloor");
			}
			else if (ray.hitTile == 102 && sceneManager.GetActiveScene()->GetName() == "SecondFloor")
			{
				sceneManager.LoadScene("NewGame");
			}*/
		}
		
	}
	
	rigid->velocity = rigid->velocity.Normalize() * speed;

}

void Player::RotateControl()
{
	const Vector<double> mouseDelta = std::move(inputSystem.GetMouseMoveDelta());
	transform.eulerAngle += mouseSpeed * mouseDelta.x * Time::deltaTime;

	const double deltaIndex = inputSystem.GetMouseWheel().y;

	

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
	
	//guns也进行更新
	guns[currentGunIndex]->Update();

	renderer->DrawString("HP:"+ ToString<int>(hp)+"\nBULLET:"+ToString<int>(bulletCount), 10, 10, RED, Screen::height / 20);

}

void Player::TakeDamage(int damage)
{
	LiveEntity::TakeDamage(damage);
	painAFX.Play(false);
}

void Player::MakeGun()
{
	shotgun = Shotgun::Create(this,&bulletCount, this->camera);
	shotgun->damage = 110;
	shotgun->takeBullet = 7;

	pistol = Pistol::Create(this,&bulletCount, this->camera);
	pistol->damage = 18;
	pistol->takeBullet = 0;

	m4a1 = M4A1::Create(this,&bulletCount, this->camera);
	m4a1->damage = 25;
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
