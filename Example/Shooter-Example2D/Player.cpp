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
	
	if (noa::Input::GetKeyHold(noa::KeyCode::KEY_SPACE)) 
	{
		rigid->velocity.y = -15.0f;
	}

	if (noa::Input::GetKeyHold(noa::KeyCode::KEY_A))
	{
		rigid->velocity.x = -15;
		spriteRenderer->isFlip.x = true;
	}
	if (noa::Input::GetKeyHold(noa::KeyCode::KEY_D))
	{
		rigid->velocity.x = 15;
		spriteRenderer->isFlip.x = false;
	}

	/*if (noa::Input::GetMouseKeyHold(noa::MouseButton::RIGHT_BUTTON))
	{
		this->transform.eulerAngle += 100*noa::Time::deltaTime;
	}*/

	timer += noa::Time::deltaTime;
	if (noa::Input::GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON)&&timer>0.1f)
	{
		Bullet* bullet = bulletPool->Request();
		bullet->transform.position = this->transform.position;

		noa::Vector<double> & mousePos = noa::Input::GetMousePosition();

		noa::Vector<float> worldPos = camera->ScreenPointToWorld(mousePos.x, mousePos.y);

		noa::Vector<float> direction = (worldPos - this->transform.position).Normalize();
		
		bullet->SetDirection(direction.x,direction.y);

		timer = 0.0f;

	}

}