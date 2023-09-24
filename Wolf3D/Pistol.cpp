#include "Pistol.h"
#include "Enimy.h"
#include "WolfResource.h"
#include "Player.h"

Pistol::Pistol(Player* player, int* bulletCount, FreeCamera* camera):Gun(bulletCount,camera)
{
	this->sprite = new Sprite(
		resource.LoadSprFile("./Assets/Wolf/gun.spr")
		, Vector<int>(0.5 * pixelWidth, 0.5 * pixelWidth));

	this->audio = new Audio("./Assets/Wolf/Music/p228-1.wav", Chunk);

	this->animation = Animation::Create(player,20, false);
	//animation->LoadFromAnimationFile("./Assets/Wolf/lgun-shot.amt");
	animation->SetFrame(&wolfResource->pistolFrame);
	animation->SetFrameEvent(1, [this]()
		{

			(*(this->bulletCount)) = (*(this->bulletCount)) - takeBullet;
			if ((*(this->bulletCount)) < 0)
			{
				(*this->bulletCount) = 0;
			}
			audio->Play(false);

			Enimy* enimy = nullptr;

			for (int i = 0.5 * pixelWidth - 0.01 * pixelWidth; i <= 0.5 * pixelWidth + 0.01 * pixelWidth; i++)
			{
				enimy = this->camera->GetRayHitInfoAs<Enimy*>(i);

				if (enimy != nullptr)
				{
					break;
				}
			}

			if (enimy != nullptr)
			{
				if (enimy->rigid->tag != "Enimy")
				{
					return;
				}
				enimy->OnPain();
				enimy->TakeDamage(damage);
			}
		});
}

Pistol* Pistol::Create(Player* player, int* bulletCount, FreeCamera* camera)
{
	return new Pistol(player,bulletCount,camera);
}

void Pistol::Update()
{
	Gun::Update();
	//ÏÔÊ¾Í¼Æ¬
	const float offsetX = 0;
	const float offsetY = 0;

	this->sprite->DrawSprite(0.5 * pixelWidth, pixelHeight - 0.5 * pixelWidth + offsetY, true,true);
}

void Pistol::Delete()
{
	delete this->sprite;
	delete this->audio;
	delete this;
}
