#pragma once
#include "./../NoaEngine/NoaEngine/NoaEngine.h"
namespace noa {
	
	class EditorCamera:public noa::Camera
	{
		//��Ҫ���ڱ༭��
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
		Vector<float> offset;//��Ļ���ϽǱ�ʾ��������ϵ������
		Vector<float> visableTiles;
		float moveSpeed = 0.1f;
		float scollSpeed = 0.1f;
	public:
		//����Լ���λ��
		Transform transform;
	};
}


