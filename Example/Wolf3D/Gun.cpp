#include "Gun.h"

Gun::Gun(int* bulletCount, FreeCamera* camera)
{
	this->bulletCount = bulletCount;
	this->camera = camera;

}

Gun::~Gun()
{

}

void Gun::Delete()
{
	delete this;
}

void Gun::OnEnter()
{
}

void Gun::Update()
{
	
}

void Gun::OnExit()
{
}

void Gun::Shoot()
{
	const int nowBulletCount = *bulletCount;
	if (nowBulletCount <=0&&animation == nullptr)
	{
		return;
	}
	
	animation->Play();

}
