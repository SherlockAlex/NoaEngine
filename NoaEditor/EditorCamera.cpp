#include "EditorCamera.h"

noa::EditorCamera::EditorCamera(noa::Scene* scene):Camera(scene) 
{

}

noa::EditorCamera::~EditorCamera() {

}

void noa::EditorCamera::Update() {
	if (noa::Input::GetMouseKeyDown(noa::MouseButton::RIGHT_BUTTON))
	{
		//按住鼠标右键，拖动相机
		Vector<double>& delta = noa::Input::GetMouseMoveDelta();
		this->transform.position.x = this->transform.position.x
			- moveSpeed * static_cast<float>(delta.x);
		this->transform.position.y = this->transform.position.y
			- moveSpeed * static_cast<float>(delta.y);
	}

	offset.x = transform.position.x
		- anchor.x * worldGrid.x + 0.5f;
	offset.y = transform.position.y
		- anchor.y * worldGrid.y + 0.5f;
}

void noa::EditorCamera::Render() 
{

	for (auto& instance : spriteRendererInstances)
	{
		if (instance.actor == nullptr)
		{
			continue;
		}

		const float screenPosX =
			(instance.actor->transform.position.x
				- offset.x + instance.offset.x) * far;
		const float screenPosY =
			(instance.actor->transform.position.y
				- offset.y + instance.offset.y) * far;

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

noa::EditorCamera& noa::EditorCamera::SetFar(float value) 
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

noa::EditorCamera* noa::EditorCamera::Apply() {
	return this;
}

noa::Vector<float> noa::EditorCamera::ScreenPointToWorld(int x,int y) 
{
	//将屏幕坐标转为世界坐标
	noa::Vector<float> result;
	if (x < 0 || x >= Screen::width
		|| y < 0 || y >= Screen::height)
	{
		return result;
	}

	result.x = static_cast<float>(x / far + offset.x);
	result.y = static_cast<float>(y / far + offset.y);

	return result;
}