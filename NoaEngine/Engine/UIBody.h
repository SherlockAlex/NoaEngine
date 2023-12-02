#ifndef NOAENGINE_UIBODY_H
#define NOAENGINE_UIBODY_H

#include <vector>

#include "NObject.h"
#include "Transform.h"
#include "Core.h"

namespace noa 
{
	class NOA_API UIScript;

	class NOA_API UIBody :public noa::NoaObject
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
		bool isEnter = false;
		bool isEnterFlag = false;
		bool isExit = false;
		bool isExitFlag = false;
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

#define UIBODY(T) template<class T> friend class noa::NObject;

#endif // !NOAENGINE_UIBODY_H
