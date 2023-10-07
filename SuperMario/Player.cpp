#include "Player.h"
#include "Bullet.h"
#include "BulletPool.h"

using namespace noa;

Player::Player(Scene* scene) :LiveEntity(scene)
{
	spriteRenderer->SetSprite(&sprite);
	rigid->useGravity = false;
	rigid->tag = "Player";

}

void Player::Update()
{
	if (inputSystem.GetKeyHold(KeyCode::KEY_A))
	{
		rigid->velocity.x = -7;
	}
	if (inputSystem.GetKeyHold(KeyCode::KEY_D))
	{
		rigid->velocity.x = 7;
	}
	if (inputSystem.GetKeyHold(KeyCode::KEY_W))
	{
		rigid->velocity.y = -7;
	}
	if (inputSystem.GetKeyHold(KeyCode::KEY_S))
	{
		rigid->velocity.y = 7;
	}

	if (rigid->velocity.x < 0 && transform.position.x <= 0)
	{
		rigid->velocity.x = 0;
		transform.position.x = 0;
	}
	else if (rigid->velocity.x > 0 && transform.position.x >= 9)
	{
		rigid->velocity.x = 0;
		transform.position.x = 9;
	}

	if (rigid->velocity.y < 0 && transform.position.y <= 0)
	{
		rigid->velocity.y = 0;
		transform.position.y = 0;
	}
	else if (rigid->velocity.y > 0 && transform.position.y >= 4.5)
	{
		rigid->velocity.y = 0;
		transform.position.y = 4.5;
	}

	if (inputSystem.GetKeyHold(KeyCode::KEY_K)||inputSystem.GetMouseButton(MouseButton::LEFT_BUTTON))
	{

		delay = delay + Time::deltaTime;
		if (delay > 0.1)
		{
			shootAFX.Play(false);
			Bullet* bullet = bulletPool->Request();
			bullet->SetDirection({0,-1});
			bullet->ownTag = rigid->tag;
			bullet->SetPostion(this->transform.position);
			delay = 0;
		}
	}
}

void Player::TakeDamage(int damage) 
{
	this->hp -= damage;
	if (hp<=0) 
	{
		Debug::Log("This liveEntity is done");
	}
}