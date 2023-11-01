#include "Player.h"
#include "ResourceManager.h"
#include "BulletPool.h"

Player::Player(noa::Scene * scene):noa::Actor(scene) 
{
	this->tag = "Player";

	this->spriteRenderer->SetSprite(&ResourceManager::playerImage);
	this->spriteRenderer->SetScale(2,2);
	this->rigid->useCollision = true;
	this->rigid->useGravity = true;
	//this->rigid->damping = 0.9f;
	this->transform.position = { 3,3 };
	this->tileCollider->SetScale(1.5f,1.7f);
	this->rigid->gravityWeight = 5.5f;
	this->collider->radius = 0.7f;

	this->rigid->bodyType = noa::BodyType::KINEMATIC;

}

Player::~Player() {

}

void Player::Start() 
{

}

static float timer = 0;
void Player::Update() 
{
	//rigid->velocity = {};
	rigid->velocity.x = 0;
	if (noa::inputSystem.GetKeyHold(noa::KeyCode::KEY_W))
	{
		rigid->velocity.y = -15;
	}
	if(noa::inputSystem.GetKeyHold(noa::KeyCode::KEY_S))
	{
		rigid->velocity.y = 15;
	}
	if (noa::inputSystem.GetKeyHold(noa::KeyCode::KEY_A))
	{
		rigid->velocity.x = -15;
		spriteRenderer->isFlip.x = true;
	}
	if (noa::inputSystem.GetKeyHold(noa::KeyCode::KEY_D))
	{
		rigid->velocity.x = 15;
		spriteRenderer->isFlip.x = false;
	}

	if (noa::inputSystem.GetMouseKeyHold(noa::MouseButton::RIGHT_BUTTON)) 
	{
		this->transform.eulerAngle += 100*noa::Time::deltaTime;
	}

	timer = timer + noa::Time::deltaTime;
	if (noa::inputSystem.GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON))
	{

		if (timer>0.08f)
		{

			//Éä»÷
			const noa::Vector<double>& screenPos = noa::inputSystem.GetMousePosition();
			const noa::Vector<float>& worldPos = camera->ScreenPointToWorld(
				static_cast<float>(screenPos.x), 
				static_cast<float>(screenPos.y));
			const noa::Vector<float>& dir = worldPos - this->transform.position;

			Bullet* bullet = bulletPool->Request();
			bullet->transform.position = transform.position;
			bullet->SetDirection(dir.x, dir.y);
			timer = 0;
		}
		
	}

}