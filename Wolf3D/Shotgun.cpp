#include "Shotgun.h"
#include "Enimy.h"
#include "WolfResource.h"

Shotgun::Shotgun(int* bulletCount,FreeCamera * camera):Gun(bulletCount,camera)
{
	this->sprite =new Sprite(
		resource.LoadSprFile("./Assets/Wolf/gun.spr")
		, Vector<int>(0.25 * pixelWidth, 0.25 * pixelWidth));

	this->audio =new Audio("./Assets/Wolf/Music/shotgun.wav", Chunk);

	this->animation = new Animation(7.5, false);
	//animation->LoadFromAnimationFile("./Assets/Wolf/gun-shot.amt");
	animation->SetFrame(&wolfResource.shotgunFrame);
	animation->SetFrameEvent(1, [this]()
		{

			(*(this->bulletCount)) = (*(this->bulletCount)) - takeBullet;
			if ((*(this->bulletCount))<0)
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

void Shotgun::Update()
{
	Gun::Update();
	//��ʾͼƬ
	const float offsetX = 0;
	const float offsetY = 0;

	this->sprite->DrawSprite(0.5 * pixelWidth - 0.5 * 0.25 * pixelWidth + offsetX, pixelHeight - 0.25 * pixelWidth + offsetY, true);
}
