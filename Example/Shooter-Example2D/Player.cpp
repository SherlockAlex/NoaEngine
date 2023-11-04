#include "Player.h"
#include "ResourceManager.h"
#include "BulletPool.h"

Player::Player(noa::Scene * scene):noa::Actor(scene) 
{
	this->tag = "Player";

	this->spriteRenderer->SetSprite(&ResourceManager::playerImage);
	this->spriteRenderer->SetScale(2,2);
	this->rigid->useCollision = true;
	this->rigid->useGravity = false;
	this->rigid->damping = 0.0f;
	this->transform.position = { 3,3 };
	this->tileCollider->SetScale(1.5f,1.7f);
	this->rigid->gravityWeight = 5.5f;
	this->collider->radius = 1.0f;

	this->rigid->SetBodyType(noa::BodyType::DYNAMIC);

}

Player::~Player() {

}

void Player::Start() 
{

}

static float timer = 0;
void Player::Update() 
{
	rigid->velocity = {};
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

}