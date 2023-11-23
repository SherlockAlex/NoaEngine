#ifndef NOAENGINE_UICOMPONENT_H
#define NOAENGINE_UICOMPONENT_H

#include "Transform.h"

namespace noa {

	class UIContainer;

	class UIComponent
	{
	protected:
		bool active = false;
		friend class UIContainer;
		friend class UIDocumentComponent;

		UITransform globalTransform;
	public:
		std::string id = "ui_component";
		UITransform transform;
		Vector<float> anchor = { 0.5f,0.5f };
	protected:
		UITransform fatherTransform;	// 父节点的绝对路径
		UIComponent(UIContainer* canvas);
		virtual ~UIComponent();

	private:
		void Delete(UIComponent*& ptr);

	protected:

		virtual void Start() = 0;
		virtual void Update() = 0;		//负责UI的交互逻辑
		virtual void Render() = 0;		//负责UI的绘制

	public:

		void SetGlobalPosition(int x, int y);
		void SetActiveInContainer(bool active);
		bool GetActive();
	};
}

#endif // !NOAENGINE_UICOMPONENT_H

