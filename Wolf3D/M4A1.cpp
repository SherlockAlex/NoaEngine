#include "M4A1.h"
#include "Enimy.h"
#include "WolfResource.h"
#include "Player.h"

M4A1::M4A1(Player* player, int* bulletCount, FreeCamera* camera) :Gun(bulletCount, camera)
{

	this->audio = new Audio("./Assets/Wolf/Music/m4a1.wav", AudioType::CHUNK);

	this->animation = Animation::Create(player,45, false);
	this->player = player;
	//animation->LoadFromAnimationFile("./Assets/Wolf/lgun-shot.amt");
	animation->SetFrame(&wolfResource->m4a1Frame);
	animation->SetFrameEvent(2, [this]()
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

	this->spriteGPU = new SpriteGPU(sprite);

	canvas = UICanvasComponent::Create(player);
	image = Image::Create(canvas);
	image->transform.scale = Vector<int>(0.5 * Screen::width, 0.5 * Screen::width);
	image->SetSprite(this->sprite);
	image->isFilpX = true;
}

M4A1* M4A1::Create(Player* player, int* bulletCount, FreeCamera* camera)
{
	return new M4A1(player,bulletCount,camera);
}

void M4A1::Update()
{
	Gun::Update();
	//显示图片到屏幕上
	//一种是使用SpriteRenderer
	//还有一种是使用Image
	const float offsetX = 30 * (sinf((player->rigid->velocity.SqrMagnitude() != 0) * Time::time * 3) + 1);
	const float offsetY =15*(sinf((player->rigid->velocity.SqrMagnitude() != 0)* Time::time *6)+1);

	image->transform.position = { 
		static_cast<int>(0.5 * Screen::width + offsetX)
		, static_cast<int>(Screen::height - sprite->scale.y + offsetY)
	};
	
	//this->spriteGPU->DrawSprite(0.5 * Screen::width + offsetX, Screen::height - sprite->scale.y + offsetY,true,0.0f);
	
}

void M4A1::Delete()
{
	delete this->spriteGPU;
	delete this->sprite;
	delete this->audio;
	delete this;
}
