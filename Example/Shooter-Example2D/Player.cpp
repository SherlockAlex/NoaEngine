#include "Player.h"
#include "ResourceManager.h"

Player::Player(noa::Scene * scene):noa::Actor(scene) 
{
	this->spriteRenderer->SetSprite(&ResourceManager::playerImage);
	this->spriteRenderer->SetScale(2,2);
	this->rigid->useCollision = true;
	this->rigid->useGravity = false;
	this->rigid->damping = 0;
	this->rigid->SetTileColliderScale(1,1);
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

	noa::renderer->DrawString(noa::ToString<float>(1.0f/noa::Time::deltaTime),10,10,noa::RED,50);

}