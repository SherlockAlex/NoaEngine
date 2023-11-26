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

}


#endif // !NOAENGINE_CAMERA_H


