#include "Pistol.h"
#include "Enimy.h"
#include "WolfResource.h"
#include "Player.h"

Pistol::Pistol(Player* player, int* bulletCount, FreeCamera* camera):Gun(bulletCount,camera)
{
	

	this->audio = new Audio("./Assets/Wolf/Music/p228-1.wav", Chunk);

	this->animation = Animation::Create(player,30, false);
	this->player = player;
	

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

	this->sprite = new Sprite(
		animation->GetCurrentFrameImage()
		, Vector<int>(0.5 * pixelWidth, 0.5 * pixelWidth));

	this->spriteGPU = new SpriteGPU(sprite);

}

Pistol* Pistol::Create(Player* player, int* bulletCount, FreeCamera* camera)
{
	return new Pistol(player,bulletCount,camera);
}

void Pistol::Update()
{
	Gun::Update();
	//��ʾͼƬ
	const float offsetX = 30 * (sinf((player->rigid->velocity.SqrMagnitude() != 0) * Time::time * 3) + 1);
	const float offsetY = 15 * (sinf((player->rigid->velocity.SqrMagnitude() != 0) * Time::time * 6) + 1);

	this->spriteGPU->DrawSprite(0.5 * pixelWidth + offsetX, pixelHeight - sprite->scale.y + offsetY, true);
	//this->sprite->DrawSprite(0.5 * pixelWidth, pixelHeight - 0.5 * pixelWidth + offsetY, true,true);
}

void Pistol::Delete()
{
	delete this->spriteGPU;
	delete this->sprite;
	delete this->audio;
	delete this;
}
