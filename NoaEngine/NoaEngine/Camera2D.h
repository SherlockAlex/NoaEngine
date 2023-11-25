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

		Camera2D& SetBoundary(bool value);
		Camera2D& SetHorizonalBoundary(float min,float max);
		Camera2D& SetVerticalBoundary(float min,float max);
		Camera2D& SetFar(float value);
		Camera2D& SetAnchor(float x,float y);
		Camera2D* Apply();
		Vector<float> ScreenPointToWorld(double x, double y);

	private:
		Vector<float> worldGrid;
		Vector<float> offset;
		Vector<float> anchor;
		float far = 32.0f;//�����ߣ���ʾ32����λ���ر�ʾһ����λ����������
		
		Vector<float> horizonalBoundary;
		Vector<float> verticalBoundary;
		bool boundary = false;
	};
}

#endif // !NOAENGINE_CAMERA2D_H



