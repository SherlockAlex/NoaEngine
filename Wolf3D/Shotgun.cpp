#include "Shotgun.h"
#include "Enimy.h"
#include "WolfResource.h"
#include "Player.h"

///0,0,1.2
Shotgun::Shotgun(Player* player, int* bulletCount,FreeCamera * camera):Gun(bulletCount,camera)
{

	this->audio =new Audio("./Assets/Wolf/Music/sg552-2.wav", AudioType::CHUNK);

	this->animation = Animation::Create(player,12, false);
	this->player = player;
	

	//animation->LoadFromAnimationFile("./Assets/Wolf/gun-shot.amt");
	animation->SetFrame(&wolfResource->shotgunFrame);

	animation->SetFrameEvent(1, [this]()
		{

			(*(this->bulletCount)) = (*(this->bulletCount)) - takeBullet;
			if ((*(this->bulletCount))<0)
			{
				(*this->bulletCount) = 0;
			}
			audio->Play(false);

			Enimy* enimy = nullptr;

			for (int i = 0.5 * Screen::width - 0.01 * Screen::width; i <= 0.5 * Screen::width + 0.01 * Screen::width; i++)
			{
				enimy = this->camera->GetRayHitInfoAs<Enimy>(i);

				if (enimy != nullptr)
				{
					break;
				}
			}

			if (enimy != nullptr)
			{
				enimy->OnPain();
				enimy->TakeDamage(damage);
			}
		});

	this->sprite = new Sprite(
		animation->GetCurrentFrameImage()
		, Vector<int>(0.5 * Screen::width, 0.5 * Screen::width));

	animation->SetAnimatedSprite(this->sprite);

	this->spriteGPU = new SpriteGPU(sprite);

}

Shotgun* Shotgun::Create(Player* player, int* bulletCount, FreeCamera* camera)
{
	return new Shotgun(player,bulletCount,camera);
}

void Shotgun::Update()
{
	Gun::Update();
	//ÏÔÊ¾Í¼Æ¬
	const float offsetX = 30 * (sinf((player->rigid->velocity.SqrMagnitude() != 0) * Time::time * 3) + 1);
	const float offsetY = 15 * (sinf((player->rigid->velocity.SqrMagnitude() != 0) * Time::time * 6) + 1);

	this->spriteGPU->DrawSprite(0.5 * Screen::width + offsetX, Screen::height - sprite->scale.y + offsetY, true, 0.0f);
	//this->sprite->DrawSprite(0.5 * pixelWidth, pixelHeight - 0.5 * pixelWidth + offsetY, true,true);
}

void Shotgun::Delete()
{
	delete this->spriteGPU;
	delete this->sprite;
	delete this->audio;
	delete this;
}
