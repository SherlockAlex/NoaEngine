#include "EditorCamera.h"

noa::EditorCamera::EditorCamera(noa::Scene* scene):Camera(scene) 
{

}

noa::EditorCamera::~EditorCamera() {

}

void noa::EditorCamera::Render() 
{
	//绘制相机场景
	if (noa::Input::GetMouseKeyHold(noa::MouseButton::RIGHT_BUTTON)) 
	{
		//按住鼠标右键，拖动相机
		Vector<double>& delta = noa::Input::GetMouseMoveDelta();
		this->transform.position.x = this->transform.position.x 
			- moveSpeed*static_cast<float>(delta.x);
		this->transform.position.y = this->transform.position.y
			- moveSpeed *static_cast<float>(delta.y);
	}

}

noa::EditorCamera& noa::EditorCamera::SetTileScale(int x,int y) 
{
	this->tileScaleInScreen.x = x;
	this->tileScaleInScreen.y = y;

	visableTiles.x = static_cast<float>(noa::Screen::width) / x;
	visableTiles.y = static_cast<float>(noa::Screen::height) / y;

	return *this;
}

noa::EditorCamera* noa::EditorCamera::Apply() {
	return this;
}

noa::Vector<float> noa::EditorCamera::ScreenPointToWorld(int x,int y) 
{
	noa::Vector<float> result;
	result.x = static_cast<float>(offset.x + x * (visableTiles.x) / Screen::width);
	result.y = static_cast<float>(offset.y + x * (visableTiles.y) / Screen::height);
	return result;
}