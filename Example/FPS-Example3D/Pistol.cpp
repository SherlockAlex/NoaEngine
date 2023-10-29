#include "Pistol.h"
#include "Enimy.h"
#include "WolfResource.h"
#include "Player.h"

Pistol::Pistol(Player* player, int* bulletCount, FreeCamera* camera):Gun(bulletCount,camera)
{
	

	this->audio = AudioSource::Create(player);
	this->audio->SetAudioClip(wolfResource->pistolShotSFX);

	this->animation = Animation::Create(player);
	this->animation->SetSpeed(30);
	this->animation->SetLoop(false);
	this->player = player;
	
	animation->SetClip(&wolfResource->pistolFrame);

	animation->SetFrameEvent(1, [this]()
		{

			(*(this->bulletCount)) = (*(this->bulletCount)) - takeBullet;
			if ((*(this->bulletCount)) < 0)
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

	this->animation->SetAnimatedSprite(this->sprite);

	canvas = UICanvasComponent::Create(player);
	image = Image::Create(canvas);
	image->SetSprite(this->sprite);
	image->transform.scale = Vector<int>(0.5 * Screen::width, 0.5 * Screen::width);
	image->isFilpX = true;


}

Pistol* Pistol::Create(Player* player, int* bulletCount, FreeCamera* camera)
{
	return new Pistol(player,bulletCount,camera);
}

void Pistol::OnEnter()
{
	if (canvas)
	{
		canvas->SetActive(true);
	}
}

void Pistol::Update()
{
	//ÏÔÊ¾Í¼Æ¬
	const float offsetX = 30 * (sinf((player->rigid->velocity.SqrMagnitude() != 0) * Time::time * 3) + 1);
	const float offsetY = 15 * (sinf((player->rigid->velocity.SqrMagnitude() != 0) * Time::time * 6) + 1);

	image->transform.position = {
		static_cast<int>(0.5 * Screen::width + offsetX)
		, static_cast<int>(Screen::height - sprite->scale.y + offsetY)
	};

}

void Pistol::OnExit()
{
	if (canvas)
	{
		canvas->SetActive(false);
	}
}

void Pistol::Delete()
{
	delete this->sprite;
	delete this;
}
