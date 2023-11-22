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

		void Update() override;

		void Render() override;
		EditorCamera& SetFar(float far);
		EditorCamera* Apply();

		Vector<float> ScreenPointToWorld(int x,int y);

	private:
		Vector<float> anchor = {0.5f,0.5f};
		Vector<float> offset;//屏幕左上角表示世界坐标系的哪里
		Vector<float> worldGrid;
		float moveSpeed = 0.1f;
		float scollSpeed = 0.1f;

		float far = 32.0f;

	public:
		//相机自己的位置
		Transform transform;
	};
}


