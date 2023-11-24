#ifndef NOAENGINE_UIBODY_H
#define NOAENGINE_UIBODY_H

#include <vector>

#include "Transform.h"

namespace noa {
	class UIScript;

	class UIBody 
	{
	protected:
		virtual ~UIBody();
		void OnUpdate();
	public:
		void AddScript(UIScript* script);
		bool GetHandle();
		void SetLocalPosition(int x, int y);
		void SetLocalSize(int w,int h);
		void SetFatherTransform(const UITransform& transform);
		void SetGlobalPosition(int x, int y);
		void SetFatherAnchor(float x,float y);
		void SetActiveScale(float value);
		template<class T>
		T* GetElementByType() {
			return dynamic_cast<T*>(this);
		}

	private:
		std::vector<UIScript*> scripts;
	protected:
		UITransform transform;
		UITransform fatherTransform;
		UITransform globalTransform;
		Vector<float> anchor = { 0.5f,0.5f };
		Vector<float> fatherAnchor = { 0.5f,0.5f };
		bool handled = false;
		float activeScale = 1.0f;
	};
}

#endif // !NOAENGINE_UIBODY_H
