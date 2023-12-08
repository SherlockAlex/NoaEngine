#ifndef NOAENGINE_CAMERA2D_H
#define NOAENGINE_CAMERA2D_H

#include "Core.h"
#include "Camera.h"

namespace noa {
	class NOA_API Camera2D final:public Camera
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

		Camera2D& SetSmooth(float value);
		Camera2D& SetFollow(Actor* actor);
		Camera2D& ApplyHorizonalBoundary(bool value);
		Camera2D& ApplyVerticalBoundary(bool value);
		Camera2D& SetHorizonalBoundary(float min,float max);
		Camera2D& SetVerticalBoundary(float min,float max);
		Camera2D& SetFar(float value);
		Camera2D& SetAnchor(float x,float y);
		Camera2D* Apply();
		Vector<float> ScreenPointToWorld(double x, double y);

	private:
		Actor* follow = nullptr;
		
		Vector<float> worldGrid;
		Vector<float> offset;
		Vector<float> anchor = {0.5f,0.5f};
		float far = 32.0f;//比例尺，表示32个单位像素表示一个单位的世界坐标
		float invFar = 1.0f / far;

		Vector<float> horizonalBoundary;
		Vector<float> verticalBoundary;
		bool ableHorizonalBoundary = false;
		bool ableVerticalBoundary = false;

		float smooth = 1.0f;

	};
}

#endif // !NOAENGINE_CAMERA2D_H