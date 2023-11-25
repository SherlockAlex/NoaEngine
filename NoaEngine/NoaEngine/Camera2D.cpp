#include "Camera2D.h"
#include "Actor.h"
#include "ActorComponent.h"
#include "Camera.h"
#include "Renderer.h"
#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Graphic.h"

noa::Camera2D::Camera2D(noa::Scene* scene) :Camera(scene)
{

}

noa::Camera2D::~Camera2D() {
	
}

void noa::Camera2D::Awake() {
	
}

void noa::Camera2D::Update() {
	//offset表示的是屏幕左上方是世界坐标的什么位置

	if (follow!=nullptr) 
	{
		transform.position.x = 
			noa::Math::LinearLerp(transform.position.x,follow->transform.position.x,smooth*20.0f);
		transform.position.y =
			noa::Math::LinearLerp(transform.position.y, follow->transform.position.y, smooth *20.0f);
	}

	offset.x = transform.position.x 
		- anchor.x * worldGrid.x + 0.5f;
	offset.y = transform.position.y 
		- anchor.y * worldGrid.y + 0.5f;

	if (!boundary)
	{
		return;
	}

	//设置相机边界
	if (horizonalBoundary.y-horizonalBoundary.x>=worldGrid.x) 
	{
		if (offset.x <= horizonalBoundary.x)
		{
			offset.x = horizonalBoundary.x;
		}
		else if (offset.x >= horizonalBoundary.y - worldGrid.x)
		{
			offset.x = horizonalBoundary.y - worldGrid.x;
		}
	}
	
	if (verticalBoundary.y - verticalBoundary.x >= worldGrid.y) 
	{
		if (offset.y <= verticalBoundary.x)
		{
			offset.y = verticalBoundary.x;
		}
		else if (offset.y >= verticalBoundary.y - worldGrid.y)
		{
			offset.y = verticalBoundary.y - worldGrid.y;
		}

	}
	

}

void noa::Camera2D::Render() {
	//绘制屏幕信息

	for (auto& instance:spriteRendererInstances) 
	{
		if (instance.actor == nullptr)
		{
			continue;
		}

		const float screenPosX = 
			(instance.actor->transform.position.x
			- offset.x+instance.offset.x) * far;
		const float screenPosY = 
			(instance.actor->transform.position.y
			- offset.y+instance.offset.y) * far;

		instance.spriteGPU->DrawSprite(
			screenPosX
			, screenPosY
			, instance.scale.x * instance.spriteSize.x
			, instance.scale.y * instance.spriteSize.y
			, instance.tint
			, instance.isFlip.x
			, (instance.actor == nullptr) ? 0.0f : instance.actor->transform.eulerAngle
		);

	}

	spriteRendererInstances.clear();

}

noa::Camera2D* noa::Camera2D::Create(noa::Scene* scene)
{
	return noa::NObject<noa::Camera2D>::Create(scene);
}

noa::Camera2D& noa::Camera2D::SetSmooth(float value)
{
	this->smooth = value;
	return *this;
}

noa::Camera2D& noa::Camera2D::SetFollow(Actor* follow)
{
	this->follow = follow;
	return *this;
}

noa::Camera2D& noa::Camera2D::SetBoundary(bool value)
{
	this->boundary = value;
	return *this;
}

noa::Camera2D& noa::Camera2D::SetHorizonalBoundary(float min, float max)
{
	this->horizonalBoundary.x = min;
	this->horizonalBoundary.y = max;
	return *this;
}

noa::Camera2D& noa::Camera2D::SetVerticalBoundary(float min,float max)
{
	this->verticalBoundary.x = min;
	this->verticalBoundary.y = max;
	return *this;
}

noa::Camera2D& noa::Camera2D::SetFar(float value) 
{
	far = value;
	if (far <= 0)
	{
		this->far = 0.01f;
	}

	worldGrid = {
		static_cast<float>(Screen::width) / far,
		static_cast<float>(Screen::height) / far,
	};

	return *this;
}

noa::Camera2D& noa::Camera2D::SetAnchor(float x,float y)
{
	anchor.x = x;
	anchor.y = y;
	return *this;
}

noa::Camera2D* noa::Camera2D::Apply() {
	return this;
}

noa::Vector<float> noa::Camera2D::ScreenPointToWorld(double x,double y) 
{
	//将屏幕坐标转为世界坐标
	noa::Vector<float> result;
	if (x<0||x>=Screen::width
		||y<0||y>=Screen::height) 
	{
		return result;
	}

	result.x = static_cast<float>(x / far + offset.x);
	result.y = static_cast<float>(y / far + offset.y);

	return result;

}




