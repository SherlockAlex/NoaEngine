#ifndef NOAENGINE_CAMERA2D_H
#define NOAENGINE_CAMERA2D_H

#include "Camera.h"
namespace noa {
	class Camera2D final:public Camera
	{
	private:
		ACTOR(Camera2D)
	private:
		Camera2D(noa::Scene* scene);
		~Camera2D();

		void Awake() override;
		void Update() override;
		void Render() override;

	public:
		static Camera2D* Create(noa::Scene* scene);

		Camera2D& SetFar(float value);
		Camera2D& SetAnchor(float x,float y);
		Camera2D* Apply();
		Vector<float> ScreenPointToWorld(double x, double y);

	private:
		std::vector<Actor*> actorsInScreen;
		Vector<float> worldGrid;
		Vector<float> offset;
		Vector<float> anchor;
		float far = 32.0f;//比例尺，表示32个单位像素表示一个单位的世界坐标
	};
}

#endif // !NOAENGINE_CAMERA2D_H



