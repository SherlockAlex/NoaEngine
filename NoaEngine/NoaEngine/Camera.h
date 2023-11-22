#ifndef NOAENGINE_CAMERA_H
#define NOAENGINE_CAMERA_H

#include "Actor.h"
#include "NoaMath.h"
#include "Scene.h"
#include "NObject.h"
#include "Screen.h"

/*
* Ïà»úÄ£¿é:
*/

namespace noa {

	typedef struct Ray {
		float angle = 0.0;
		float distance = 0.0;

		Vector<int> tilePosition = { 0,0 };

		int hitTile = -1;

		Vector<float> simple = { 0,0 };
	}Ray;

	class Scene;
	class Transform;
	class Camera :public Actor
	{
	private:
		ACTOR(Camera)
	protected:
		Camera(Scene* scene);
		virtual ~Camera();
	protected:

		virtual void Render() = 0;
	};


	/*class FreeCamera final :public Camera
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
		FreeCamera(Scene* scene);
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

	};*/

}


#endif // !NOAENGINE_CAMERA_H


