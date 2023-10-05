#include "Player.h"
#include "Bullet.h"

using namespace noa;

Player::Player(Scene* scene) :Actor(scene)
{
	spriteRenderer->SetSprite(&sprite);
	rigid->useGravity = false;
}

void Player::Update()
{
	rigid->velocity.x = 0.9995f * rigid->velocity.x;
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

	if (rigid->velocity.y < 0 && transform.position.y <= 2.5)
	{
		rigid->velocity.y = 0;
		transform.position.y = 2.5;
	}
	else if (rigid->velocity.y > 0 && transform.position.y >= 4.5)
	{
		rigid->velocity.y = 0;
		transform.position.y = 4.5;
	}

	if (inputSystem.GetKeyHold(KeyCode::KEY_K))
	{

		delay = delay + Time::deltaTime;
		if (delay > 0.1)
		{
			shootAFX.Play(false);
			Bullet* bullet = NObject<Bullet>::Create<Scene*>(this->activeScene);
			bullet->SetPostion(this->transform.position);
			delay = 0;
		}
	}
}