#include "Player.h"
#include "ResourceManager.h"
#include "BulletPool.h"

Player::Player(noa::Scene * scene):noa::Actor(scene) 
{
	this->tag = "Player";

	this->rigid->useGravity = false;
	this->rigid->damping = 0.0f;
	this->transform.position = { 3,3 };
	this->rigid->gravityWeight = 5.5f;
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

	if (noa::inputSystem.GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON)) 
	{
		Bullet* bullet = bulletPool->Request();
		bullet->transform.position = this->transform.position;

		noa::Vector<double> & mousePos = noa::inputSystem.GetMousePosition();

		noa::Vector<float> worldPos = camera->ScreenPointToWorld(mousePos.x, mousePos.y);

		noa::Vector<float> direction = (worldPos - this->transform.position).Normalize();
		
		bullet->SetDirection(direction.x,direction.y);



	}

}