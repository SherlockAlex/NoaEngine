#ifndef NOAENGINE_CAMERA_H
#define NOAENGINE_CAMERA_H

#include "NoaMath.h"
#include "Scene.h"
#include "GameObject.h"

namespace noa {
	//这个是游戏的相机，用来显示游戏的画面内容
	//主要是用来渲染游戏的场景
	// 越靠前的就先被渲染
	
	extern int pixelWidth;
	extern int pixelHeight;

	class Transform;
	class Camera
	{
	protected:
		//要渲染的物品
		

	public:
		Transform* follow;
		Vector<float> position;

	public:
		Camera();
		Camera(Transform* follow);
		virtual ~Camera();
		
		
	};

	class TileMapCamera :public Camera
	{
	private:
		Vector<int> tileScale = Vector<int>(64, 64);
		Vector<float> visibleTiles;
		Vector<float> offset;
		Vector<int> followPositionOnScreen = Vector<int>(0.0, 0.0);
		
		//获取射线射中的物品
		std::vector<void*> objectBufferWithRay = std::vector<void*>(pixelWidth*pixelHeight, nullptr);
	public:
		TileMapCamera();
		TileMapCamera(Vector<int> tileScale, Transform* follow);

		/// <summary>
		/// 渲染瓦片地图到屏幕上
		/// </summary>
		/// <param name="tileMap">要渲染的瓦片地图</param>
		/// <param name="frontDelta">相机前边界偏移量</param>
		/// <param name="endDelta">相机后边界偏移量</param>
		Vector<int> Render(TileMap& tileMap,const Vector<float> & frontDelta,const Vector<float> & endDelta);
	
		//获取像素点横坐标射线存储的物品信息
		template<class T>
		T GetRayHitInfoAs(int index) {
			return (T)objectBufferWithRay[index];
		}
	};

	//光线
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
		//获取射线射中的物品
		std::vector<float> wallDistanceBuffer;
		std::vector<void*> objectBufferWithRay = std::vector<void*>(pixelWidth, nullptr);
		std::vector<Ray> rayResult = std::vector<Ray>(pixelWidth, Ray());
	public:
		float FOV = 0.25 * PI;
		const float halfFOV = FOV * 0.5;
		float viewDepth = 60;
		const float normalEyeRay = (1.0 / cosf(halfFOV));
		
		//const float normalEyeRay = 1;

	public:
		FreeCamera();
		FreeCamera(Transform* follow);

		void RenderFloor(TileMap& map,uint32_t multiColor);
		void Render(TileMap& map, bool renderFloor, Sprite * skybox,uint32_t mutiColor);
		void RenderGameObjectEnter();
		void RenderGameObject(uint32_t multiColor);

		Ray RaycastHit(int pixelX, const TileMap& map);

		//获取像素点横坐标射线存储的物品信息
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


