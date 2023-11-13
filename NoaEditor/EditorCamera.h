#pragma once
#include "./../NoaEngine/NoaEngine/NoaEngine.h"
namespace noa {
	
	class EditorCamera:public noa::Camera
	{
		//主要用于编辑器
	private:
		NOBJECT(EditorCamera)
	private:
		EditorCamera(noa::Scene* scene);
		~EditorCamera();
	public:
		void Render() override;
		EditorCamera& SetTileScale(int x,int y);
		EditorCamera* Apply();

		Vector<float> ScreenPointToWorld(int x,int y);

	private:
		Vector<int> tileScaleInScreen = {1,1};
		Vector<float> offset;//屏幕左上角表示世界坐标系的哪里
		Vector<float> visableTiles;
		float moveSpeed = 0.1f;
		float scollSpeed = 0.1f;
	public:
		//相机自己的位置
		Transform transform;
	};
}


