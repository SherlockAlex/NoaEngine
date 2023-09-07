#include "Player.h"
#include "Enimy.h"

Player::Player(TileMap* map) :Behaviour(), Rigidbody(&transform)
{
	tag = "Player";
	colliderSize = { -0.2,-0.2 };
	useGravity = false;

	damping = 0;

	vector<int> collisionTileID;
	collisionTileID.push_back(36);
	for (int i = 0; i < 108; i++)
	{
		if (i == 107)
		{
			continue;
		}

		collisionTileID.push_back(i);

	}
	SetCollisionTileID(collisionTileID);
	UpdateMap(map);

	inputSystem.inputEvent += [this]() {
		this->RotateControl();
		};
}

Player::~Player() {
	Behaviour::~Behaviour();
	Rigidbody::~Rigidbody();
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

void Player::ActorControl() {

	velocity.x = 0;
	velocity.y = 0;

	if (inputSystem.GetKeyHold(KeyW))
	{
		velocity.x += sinf(transform.eulerAngle);
		velocity.y += cosf(transform.eulerAngle);
	}

	if (inputSystem.GetKeyHold(KeyS))
	{
		velocity.x += -sinf(transform.eulerAngle);
		velocity.y += -cosf(transform.eulerAngle);
	}

	if (inputSystem.GetKeyHold(KeyA))
	{
		velocity.x += -cosf(transform.eulerAngle);
		velocity.y += sinf(transform.eulerAngle);
	}

	if (inputSystem.GetKeyHold(KeyD))
	{
		velocity.x += cosf(transform.eulerAngle);
		velocity.y += -sinf(transform.eulerAngle);
	}



	if (inputSystem.GetMouseButton(LeftButton))
	{

		//shotAFX.Play(false);
		if (bulletCount > 0)
		{
			gunShot->Play();
		}


	}

	velocity = velocity.Normalize() * speed;


}

void Player::RotateControl()
{
	if (inputSystem.GetMouseMoveState())
	{
		Vector<float> mouseDelta = inputSystem.GetMouseMoveDelta();
		transform.eulerAngle += 0.05 * mouseDelta.x * deltaTime;
	}
}

void Player::Start()
{
	//gunNormal->LoadFromAnimatorFile("./Assets/Wolf/gun-normal.amt");
	gunShot->LoadFromAnimatorFile("./Assets/Wolf/lgun-shot.amt");
	gunShot->SetFrameEvent(2, [this]()
		{
			bulletCount--;
			shotAFX.Play(false);

			Enimy* enimy = nullptr;

			for (int i = 0.5 * pixelWidth - 0.01 * pixelWidth; i <= 0.5 * pixelWidth + 0.01 * pixelWidth; i++)
			{
				enimy = camera->GetRayHitInfoAs<Enimy*>(i);

				if (enimy != nullptr)
				{
					break;
				}
			}

			if (enimy != nullptr)
			{
				if (enimy->tag != "Enimy")
				{
					return;
				}
				enimy->TakeDamage(20);
			}


			//Debug("shot");
		});

}

void Player::Update()
{
	ActorControl();
	gunSprite.UpdateImage(gunShot->GetCurrentFrameImage());
	gunSprite.DrawSprite(0.5 * pixelWidth, pixelHeight - 0.25 * pixelWidth, true);
	renderer.DrawString("hp:"+to_string(this->hp)+"\nbullet:" + to_string(bulletCount), 0, 0, RED, pixelHeight / 20);
}