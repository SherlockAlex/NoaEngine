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
		template<class T>
		T* GetElementByType() {
			return dynamic_cast<T*>(this);
		}

	private:
		std::vector<UIScript*> scripts;
	protected:

		//要定义globalTransform是绝对坐标
		//是锚点为{0,0}的那个坐标

		//transform是{anchor.x,anchor.y}
		//相对于fatherTransform + father.Anchor*fatherSize

		UITransform transform;
		UITransform fatherTransform;
		UITransform globalTransform;
		Vector<float> anchor = { 0.5f,0.5f };
		Vector<float> fatherAnchor = { 0.5f,0.5f };

		bool handled = false;
	};
}

#endif // !NOAENGINE_UIBODY_H
