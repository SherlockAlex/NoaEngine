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
	offset.x = transform.position.x 
		- anchor.x * worldGrid.x + 0.5f;
	offset.y = transform.position.y 
		- anchor.y * worldGrid.y + 0.5f;
}

void noa::Camera2D::Render() {
	//绘制屏幕信息

	for (auto& instance:spriteRendererInstances) 
	{
		if (instance.actor == nullptr)
		{
			continue;
		}

		const float screenPosX = (instance.actor->transform.position.x
			- offset.x) * far;
		const float screenPosY = (instance.actor->transform.position.y
			- offset.y) * far;

		instance.spriteGPU->DrawSprite(
			screenPosX
			, screenPosY
			, instance.scale.x * instance.sprite->w
			, instance.scale.y * instance.sprite->h
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




