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
	protected:

	public:
		Transform* follow = nullptr;
		Vector<float> position;

	protected:
		Camera();
		virtual ~Camera();
	public:
		void SetFollow(Transform* follow);
		
	};

	//静态相机
	class SpriteGPU;
	class StaticCamera final :public Camera 
	{
	private:
		//背景图片
		SpriteGPU * background = nullptr;
	public:
		StaticCamera(Sprite * sprite);
		void Render();
	};

	class TileMapCamera final:public Camera
	{
	private:
		Vector<int> tileScale = Vector<int>(64, 64);
		Vector<float> visibleTiles;
		Vector<float> offset;
		Vector<int> followPositionOnScreen = Vector<int>(0, 0);
		
		std::vector<void*> objectBufferWithRay = std::vector<void*>(pixelWidth*pixelHeight, nullptr);
	public:
		TileMapCamera();

		void SetTileScale(Vector<int> tileScale);

		Vector<int> Render(TileMap& tileMap,const Vector<float> & frontDelta,const Vector<float> & endDelta);

		template<class T>
		T GetRayHitInfoAs(int index) {
			return (T)objectBufferWithRay[index];
		}
	};

	typedef struct Ray {
		float angle = 0.0;
		float distance = 0.0;
		
		Vector<int> tilePosition = {0,0};

		int hitTile = -1;

		Vector<float> simple = {0,0};
	}Ray;

	class FreeCamera final:public Camera
	{
	protected:
		std::vector<float> wallDistanceBuffer;
		std::vector<void*> objectBufferWithRay = std::vector<void*>(pixelWidth, nullptr);
		std::vector<Ray> rayResult = std::vector<Ray>(pixelWidth, Ray());
	public:
		float FOV = static_cast<float>(0.25 * PI);
		const float halfFOV = static_cast<float>(FOV * 0.5f);
		float viewDepth = 60;
		const float normalEyeRay = (1.0f / cosf(halfFOV));

	public:
		FreeCamera();
	public:
		void RenderFloor(TileMap& map,uint32_t multiColor);
		void Render(TileMap& map, bool renderFloor, Sprite * skybox,uint32_t mutiColor);
		void RenderGameObjectEnter();
		void RenderGameObject(uint32_t multiColor);

		Ray RaycastHit(int pixelX, const TileMap& map);

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


