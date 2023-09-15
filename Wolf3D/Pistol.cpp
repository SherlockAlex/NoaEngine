#include "Pistol.h"
#include "Enimy.h"
#include "WolfResource.h"

Pistol::Pistol(int* bulletCount, FreeCamera* camera):Gun(bulletCount,camera)
{
	this->sprite = new Sprite(
		resource.LoadSprFile("./Assets/Wolf/gun.spr")
		, Vector<int>(0.5 * pixelWidth, 0.5 * pixelWidth));

	this->audio = new Audio("./Assets/Wolf/Music/handleGunShot.mp3", Chunk);

	this->animation = new Animation(15, false);
	//animation->LoadFromAnimationFile("./Assets/Wolf/lgun-shot.amt");
	animation->SetFrame(&wolfResource.pistolFrame);
	animation->SetFrameEvent(2, [this]()
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
				if (enimy->tag != "Enimy")
				{
					return;
				}
				enimy->OnPain();
				enimy->TakeDamage(damage);
			}
		});
}

void Pistol::Update()
{
	Gun::Update();
	//ÏÔÊ¾Í¼Æ¬
	const float offsetX = 0;
	const float offsetY = 0;

	this->sprite->DrawSprite(0.5 * pixelWidth - 0.5 * 0.5 * pixelWidth + offsetX, pixelHeight - 0.5 * pixelWidth + offsetY, true);
}
