#include "Player.h"
#include "ResourceManager.h"
#include "BulletPool.h"

Player::Player(noa::Scene * scene):noa::Actor(scene) 
{
	this->spriteRenderer->SetSprite(&ResourceManager::playerImage);
	this->spriteRenderer->SetScale(2,2);
	this->rigid->useCollision = true;
	this->rigid->useGravity = false;
	this->rigid->damping = 0;
	this->rigid->SetTileColliderScale(1.5,1.7);
	this->transform.position = { 3,3 };
}

Player::~Player() {

}

void Player::Start() 
{

}

void Player::Update() 
{
	rigid->velocity = {};
	if (noa::inputSystem.GetKeyHold(noa::KeyCode::KEY_W))
	{
		rigid->velocity.y = -10;
	}
	if(noa::inputSystem.GetKeyHold(noa::KeyCode::KEY_S))
	{
		rigid->velocity.y = 10;
	}
	if (noa::inputSystem.GetKeyHold(noa::KeyCode::KEY_A))
	{
		rigid->velocity.x = -10;
		spriteRenderer->isFlip.x = true;
	}
	if (noa::inputSystem.GetKeyHold(noa::KeyCode::KEY_D))
	{
		rigid->velocity.x = 10;
		spriteRenderer->isFlip.x = false;
	}

	if (noa::inputSystem.GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON))
	{
		//Éä»÷
		const noa::Vector<double>& screenPos = noa::inputSystem.GetMousePosition();
		const noa::Vector<float>& worldPos = camera->ScreenPointToWorld(screenPos.x, screenPos.y);
		const noa::Vector<float> & dir = worldPos - this->transform.position;

		Bullet* bullet = bulletPool->Request();
		bullet->transform.position = transform.position;
		bullet->SetDirection(dir.x,dir.y);

		noa::Debug::Log(noa::ToString<double>(screenPos));

	}

	

	//noa::renderer->DrawString(noa::ToString<float>(1.0f/noa::Time::deltaTime),10,10,noa::RED,50);

}