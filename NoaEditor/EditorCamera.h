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

		void Update() override;

		void Render() override;
		EditorCamera& SetFar(float far);
		EditorCamera* Apply();

		Vector<float> ScreenPointToWorld(int x,int y);

	private:
		Vector<float> anchor = {0.5f,0.5f};
		Vector<float> offset;//��Ļ���ϽǱ�ʾ��������ϵ������
		Vector<float> worldGrid;
		float moveSpeed = 0.1f;
		float scollSpeed = 0.1f;

		float far = 32.0f;

	public:
		//����Լ���λ��
		Transform transform;
	};
}


