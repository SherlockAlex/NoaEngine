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
	protected:
		//Ҫ��Ⱦ����Ʒ
		

	public:
		Transform* follow = nullptr;
		Vector<float> position;

	protected:
		Camera();
		virtual ~Camera();
	public:
		void SetFollow(Transform* follow);
		
	};

	class TileMapCamera :public Camera
	{
	private:
		Vector<int> tileScale = Vector<int>(64, 64);
		Vector<float> visibleTiles;
		Vector<float> offset;
		Vector<int> followPositionOnScreen = Vector<int>(0, 0);
		
		//��ȡ�������е���Ʒ
		std::vector<void*> objectBufferWithRay = std::vector<void*>(pixelWidth*pixelHeight, nullptr);
	public:
		TileMapCamera();

		void SetTileScale(Vector<int> tileScale);

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
		
		Vector<int> tilePosition = {0,0};

		int hitTile = -1;

		Vector<float> simple = {0,0};
	}Ray;

	class FreeCamera :public Camera
	{
	protected:
		//��ȡ�������е���Ʒ
		std::vector<float> wallDistanceBuffer;
		std::vector<void*> objectBufferWithRay = std::vector<void*>(pixelWidth, nullptr);
		std::vector<Ray> rayResult = std::vector<Ray>(pixelWidth, Ray());
	public:
		float FOV = static_cast<float>(0.25 * PI);
		const float halfFOV = static_cast<float>(FOV * 0.5f);
		float viewDepth = 60;
		const float normalEyeRay = (1.0f / cosf(halfFOV));
		
		//const float normalEyeRay = 1;

	public:
		FreeCamera();
	public:
		void RenderFloor(TileMap& map,uint32_t multiColor);
		void Render(TileMap& map, bool renderFloor, Sprite * skybox,uint32_t mutiColor);
		void RenderGameObjectEnter();
		void RenderGameObject(uint32_t multiColor);

		Ray RaycastHit(int pixelX, const TileMap& map);

		//��ȡ���ص���������ߴ洢����Ʒ��Ϣ
		template<class T>
		T GetRayHitInfoAs(int index) {
			return (T)objectBufferWithRay[index];
		}

		Ray GetRayInfo(int index) {
			return rayResult[index];
		}

	};

	

}


#endif // !NOAENGINE_CAMERA_H


