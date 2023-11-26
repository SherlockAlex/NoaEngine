#include "Player.h"
#include "ResourceManager.h"
#include "BulletPool.h"

Player::Player(noa::Scene * scene):noa::Actor(scene) 
{
	this->tag = "Player";
	this->transform.position = { 3,3 };
}

Player::~Player() {

}

void Player::Start() 
{

}

static float timer = 0;
void Player::Update() 
{
	rigid->velocity.x = noa::Math::LinearLerp(rigid->velocity.x,0,50);
	
	if (noa::Input::GetKeyDown(noa::KeyCode::KEY_SPACE)) 
	{
		rigid->velocity.y = -15.0f;
	}

	if (noa::Input::GetKeyDown(noa::KeyCode::KEY_A))
	{
		rigid->velocity.x = -15;
		spriteRenderer->isFlip.x = true;
	}

	if (noa::Input::GetKeyDown(noa::KeyCode::KEY_D))
	{
		rigid->velocity.x = 15;
		spriteRenderer->isFlip.x = false;
	}

	if (noa::Input::GetKeyUp(noa::KeyCode::KEY_K)) 
	{
		this->hp = hp - 1;
	}

	if (noa::Input::GetMouseKeyDown(noa::MouseButton::RIGHT_BUTTON))
	{
		this->transform.eulerAngle += 1*noa::Time::deltaTime;
	}

	timer += noa::Time::deltaTime;
	if (noa::Input::GetKeyDown(noa::KeyCode::KEY_K)&&timer>0.1f)
	{
		Bullet* bullet = bulletPool->Request();
		bullet->transform.position = this->transform.position;

		noa::Vector<float> direction = { spriteRenderer->isFlip.x?-1.0f:1.0f,0.0f };
		
		bullet->SetDirection(direction.x,direction.y);

		timer = 0.0f;

	}

}