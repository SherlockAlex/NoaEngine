#ifndef NOAENGINE_CAMERA_H
#define NOAENGINE_CAMERA_H

#include "NoaMath.h"
#include "Scene.h"
#include "NObject.h"
#include "Screen.h"

/*
* 相机模块:
*/

namespace noa {

	typedef struct Ray {
		float angle = 0.0;
		float distance = 0.0;

		Vector<int> tilePosition = { 0,0 };

		int hitTile = -1;

		Vector<float> simple = { 0,0 };
	}Ray;

	class Actor;
	class Scene;
	class Transform;
	class Camera:public Actor
	{
	protected:
		friend class Scene;
		friend class SceneManager;	
		
	public:
		Vector<float> position;

	protected:
		Camera(Scene * scene);
		virtual ~Camera();
	private:
		void Delete(Camera*& ptr);
	protected:

		virtual void Update();

		// 用户可以覆写自己的相机类
		virtual void Render() = 0;
	};

	class SpriteGPU;
	class StaticCamera final :public Camera 
	{
		NOBJECT(StaticCamera)

	private:
		//背景图片
		std::shared_ptr<SpriteGPU> background = nullptr;

		Vector<int> tileScale = Vector<int>(64, 64);

		Vector<float> offset = Vector<float>(10, 10);
		Vector<float> visibleTiles;
		
		StaticCamera(Scene * scene);

	public:
		static StaticCamera* Create(Scene* scene);

		void SetBackground(Sprite * sprite);
		void SetTileScale(const Vector<int> & tileScale);
	private:
		void Render() override;
	};

	class TileMapCamera final:public Camera
	{
		NOBJECT(TileMapCamera)
	private:

		Transform* follow = nullptr;

		TileMap* tileMap = nullptr;

		Vector<int> tileScale = Vector<int>(64, 64);	//一个tile由多少个像素显示
		Vector<float> visibleTiles;						//相机可见的tile数量
		Vector<float> offset;							//相机偏移量，表示相机左上角表示世界坐标的位置
		
		std::vector<NoaObject*> objectBufferWithRay = std::vector<NoaObject*>(
			Screen::width*Screen::height, nullptr
		);

		TileMapCamera(Scene * scene);

	public:
		
		static TileMapCamera * Create(Scene * scene);

		TileMapCamera& SetTileScale(uint32_t w,uint32_t h);
		TileMapCamera& SetTileMap(TileMap * tileMap);
		TileMapCamera& SetFollow(Actor* actor);
		TileMapCamera* Apply();


		Vector<float> ScreenPointToWorld(float x,float y);

		template<class T>
		T* GetRayHitInfoAs(int index) {
			if (index<0||index>=objectBufferWithRay.size()) 
			{
				return nullptr;
			}
			return dynamic_cast<T*>(objectBufferWithRay[index]);
		}

		template<class T>
		T* GetRayHitInfoAs(int x,int y) {
			if (x<0||x>=static_cast<int>(noa::Screen::width)
				||y<0||y>=static_cast<int>(noa::Screen::height)) 
			{
				return nullptr;
			}
			return dynamic_cast<T*>(objectBufferWithRay[
				static_cast<std::vector<noa::NoaObject*, std::allocator<noa::NoaObject*>>
					::size_type>(y) 
					* noa::Screen::width + x]);
		}

	private:
		void Render() override;
	};

	class FreeCamera final:public Camera
	{
		NOBJECT(FreeCamera)
	protected:
		std::vector<float> wallDistanceBuffer;
		std::vector<NoaObject*> objectBufferWithRay = std::vector<NoaObject*>(Screen::width, nullptr);
		std::vector<Ray> rayResult = std::vector<Ray>(Screen::width, Ray());

	private:
		TileMap* map = nullptr;
		Sprite* skybox = nullptr;
		Transform* follow = nullptr;

	public:
		float FOV = static_cast<float>(0.25 * PI);
		const float halfFOV = static_cast<float>(FOV * 0.5f);
		float viewDepth = 60;
		const float normalEyeRay = (1.0f / cosf(halfFOV));
		bool renderFloor = false;

	private:
		FreeCamera(Scene * scene);
	public:
		static FreeCamera* Create(Scene* scene);

		FreeCamera& SetFollow(Actor* actor);
		FreeCamera& SetTileMap(TileMap* map);
		FreeCamera& SetSkybox(Sprite* skybox);
		FreeCamera* Apply();

		Ray RaycastHit(int pixelX);

		template<class T>
		T* GetRayHitInfoAs(int index) {
			return dynamic_cast<T*>(objectBufferWithRay[index]);
		}

		Ray GetRayInfo(int index) {
			return rayResult[index];
		}

	private:
		void RenderFloor();
		void Render() override;
		void RenderGameObjectEnter();
		void RenderGameObject();

	};

	

}


#endif // !NOAENGINE_CAMERA_H


