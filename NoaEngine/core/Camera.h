#ifndef NOAENGINE_CAMERA_H
#define NOAENGINE_CAMERA_H

#include "NoaMath.h"
#include "Scene.h"


namespace noa {
	//这个是游戏的相机，用来显示游戏的画面内容
	//主要是用来渲染游戏的场景
	// 越靠前的就先被渲染
	
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
		/// 相机构造函数
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

		//获取像素点横坐标射线存储的物品信息
		template<class T>
		T GetRayHitInfoAs(int index) {
			return (T)objectBufferWithRay[index];
		}

	};

	

}


#endif // !NOAENGINE_CAMERA_H


