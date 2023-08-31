#ifndef NOAENGINE_CAMERA_H
#define NOAENGINE_CAMERA_H

#include "NoaMath.h"
#include "Scene.h"

namespace noa {
	//这个是游戏的相机，用来显示游戏的画面内容
	//主要是用来渲染游戏的场景
	// 越靠前的就先被渲染
	class Camera
	{
	
	public:
		Vector<float>* follow = nullptr;
		Vector<float> position;

	public:
		/// <summary>
		/// 相机构造函数
		/// </summary>
		/// <param name="follow"></param>
		Camera();
		Camera(Vector<float> * follow);
		~Camera();
	};

	class TileMapCamera :public Camera
	{
	private:
		Vector<int> tileScale = Vector<int>(64, 64);
		Vector<float> visibleTiles;
		Vector<float> offset;
		Vector<int> followPositionOnScreen = Vector<int>(0.0, 0.0);
	public:
		TileMapCamera();
		TileMapCamera(Vector<int> tileScale, Vector<float>* follow);
		
		/// <summary>
		/// 渲染瓦片地图到屏幕上
		/// </summary>
		/// <param name="tileMap">要渲染的瓦片地图</param>
		/// <param name="frontDelta">相机前边界偏移量</param>
		/// <param name="endDelta">相机后边界偏移量</param>
		Vector<int> Render(TileMap& tileMap,Vector<float> & frontDelta,Vector<float> & endDelta);
	};

}


#endif // !NOAENGINE_CAMERA_H


