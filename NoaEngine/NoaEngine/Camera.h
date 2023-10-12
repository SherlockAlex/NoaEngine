#ifndef NOAENGINE_CAMERA_H
#define NOAENGINE_CAMERA_H

#include "NoaMath.h"
#include "Scene.h"

/*
* Ïà»úÄ£¿é:
*/

namespace noa {
	
	extern int pixelWidth;
	extern int pixelHeight;

	class Scene;
	class Transform;
	class Camera
	{
	protected:
		friend class Scene;
		friend class SceneManager;
	public:
		Transform* follow = nullptr;
		Vector<float> position;

	protected:
		Camera(Scene * scene);
		virtual ~Camera();
	private:
		void Delete(Camera*& ptr);
	public:
		

		void SetFollow(Transform* follow);
	protected:
		virtual void Render() = 0;
	};

	class SpriteGPU;
	class StaticCamera final :public Camera 
	{
	private:
		//±³¾°Í¼Æ¬
		SpriteGPU * background = nullptr;

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
	private:
		TileMap* tileMap = nullptr;
		Vector<float> frontDelta = { 0.0f,0.0f };
		Vector<float> endDelta = { 0.0f,0.0f };

		Vector<int> tileScale = Vector<int>(64, 64);
		Vector<float> visibleTiles;
		Vector<float> offset;
		Vector<int> followPositionOnScreen = Vector<int>(0, 0);
		
		std::vector<void*> objectBufferWithRay = std::vector<void*>(pixelWidth*pixelHeight, nullptr);

		TileMapCamera(Scene * scene);

	public:
		
		static TileMapCamera * Create(Scene * scene);

		void SetTileScale(Vector<int> tileScale);

		

		template<class T>
		T GetRayHitInfoAs(int index) {
			return (T)objectBufferWithRay[index];
		}

	private:
		void Render() override;
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

	private:
		TileMap* map = nullptr;
		Sprite* skybox = nullptr;

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


		

		Ray RaycastHit(int pixelX);

		template<class T>
		T GetRayHitInfoAs(int index) {
			return (T)objectBufferWithRay[index];
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


