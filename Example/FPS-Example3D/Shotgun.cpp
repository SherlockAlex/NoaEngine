#include "Shotgun.h"
#include "Enimy.h"
#include "WolfResource.h"
#include "Player.h"

///0,0,1.2
Shotgun::Shotgun(Player* player, int* bulletCount,FreeCamera * camera):Gun(bulletCount,camera)
{

	this->audio =AudioSource::Create(player);
	this->audio->SetAudioClip(wolfResource->shotgunShotSFX);

	this->animation = Animation::Create(player,12, false);
	this->player = player;
	
	animation->SetClip(&wolfResource->shotgunFrame);

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

	canvas = UICanvasComponent::Create(player);
	image = Image::Create(canvas);
	image->transform.scale = Vector<int>(0.5 * Screen::width, 0.5 * Screen::width);
	image->SetSprite(this->sprite);
	image->isFilpX = true;

}

Shotgun* Shotgun::Create(Player* player, int* bulletCount, FreeCamera* camera)
{
	return new Shotgun(player,bulletCount,camera);
}

void Shotgun::OnEnter()
{
	if (canvas)
	{
		canvas->SetActive(true);
	}
}

void Shotgun::Update()
{
	//ÏÔÊ¾Í¼Æ¬
	const float offsetX = 30 * (sinf((player->rigid->velocity.SqrMagnitude() != 0) * Time::time * 3) + 1);
	const float offsetY = 15 * (sinf((player->rigid->velocity.SqrMagnitude() != 0) * Time::time * 6) + 1);

	image->transform.position = { 
		static_cast<int>(0.5 * Screen::width + offsetX)
		, static_cast<int>(Screen::height - sprite->scale.y + offsetY)
	};

	
}

void Shotgun::OnExit()
{
	if (canvas)
	{
		canvas->SetActive(false);
	}
}

void Shotgun::Delete()
{
	delete this->sprite;
	delete this;
}
