#ifndef NOAENGINE_CAMERA_H
#define NOAENGINE_CAMERA_H

#include "NoaMath.h"
#include "Scene.h"


namespace noa {
	//�������Ϸ�������������ʾ��Ϸ�Ļ�������
	//��Ҫ��������Ⱦ��Ϸ�ĳ���
	// Խ��ǰ�ľ��ȱ���Ⱦ
	
	extern int pixelWidth;
	extern int pixelHeight;

	class Transform;
	class Camera
	{
	public:
		Transform* follow;
		//Vector<float>* follow = nullptr;
		Vector<float> position;

	public:
		/// <summary>
		/// ������캯��
		/// </summary>
		/// <param name="follow"></param>
		Camera();
		Camera(Transform* follow);
		~Camera();
		
	};

	class TileMapCamera :public Camera
	{
	private:
		Vector<int> tileScale = Vector<int>(64, 64);
		Vector<float> visibleTiles;
		Vector<float> offset;
		Vector<int> followPositionOnScreen = Vector<int>(0.0, 0.0);
		
		vector<void*> objectBufferWithRay = vector<void*>(pixelWidth*pixelHeight, nullptr);
	public:
		TileMapCamera();
		TileMapCamera(Vector<int> tileScale, Transform* follow);

		/// <summary>
		/// ��Ⱦ��Ƭ��ͼ����Ļ��
		/// </summary>
		/// <param name="tileMap">Ҫ��Ⱦ����Ƭ��ͼ</param>
		/// <param name="frontDelta">���ǰ�߽�ƫ����</param>
		/// <param name="endDelta">�����߽�ƫ����</param>
		Vector<int> Render(TileMap& tileMap,const Vector<float> & frontDelta,const Vector<float> & endDelta);
	
		//��ȡ���ص���������ߴ洢����Ʒ��Ϣ
		template<class T>
		T GetRayHitInfoAs(int index) {
			return (T)objectBufferWithRay[index];
		}
	};

	//����
	typedef struct Ray {
		float angle = 0.0;
		float distance = 0.0;
		
		int hitTile = -1;

		Vector<float> simple;
	}Ray;

	class FreeCamera :public Camera
	{
	protected:
		vector<void*> objectBufferWithRay = vector<void*>(pixelWidth, nullptr);

	public:
		float FOV = 0.25 * PI;
		const float halfFOV = FOV * 0.5;
		float viewDepth = 60;
		const float normalEyeRay = (1.0 / cosf(halfFOV));
		
		//const float normalEyeRay = 1;

	public:
		FreeCamera();
		FreeCamera(Transform* follow);

		void RenderFloor(TileMap& map);
		void Render(TileMap& map, bool renderFloor, const Sprite& skybox);
		void Render(TileMap& map, bool renderFloor);
		void RenderGameObject();

		Ray RaycastHit(int pixelX, const TileMap& map);

		//��ȡ���ص���������ߴ洢����Ʒ��Ϣ
		template<class T>
		T GetRayHitInfoAs(int index) {
			return (T)objectBufferWithRay[index];
		}

	};

	

}


#endif // !NOAENGINE_CAMERA_H


