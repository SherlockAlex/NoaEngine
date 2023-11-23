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
		UITransform fatherTransform;	// ���ڵ�ľ���·��
		UIComponent(UIContainer* canvas);
		virtual ~UIComponent();

	private:
		void Delete(UIComponent*& ptr);

	protected:

		virtual void Start() = 0;
		virtual void Update() = 0;		//����UI�Ľ����߼�
		virtual void Render() = 0;		//����UI�Ļ���

	public:

		void SetGlobalPosition(int x, int y);
		void SetActiveInContainer(bool active);
		bool GetActive();
	};
}

#endif // !NOAENGINE_UICOMPONENT_H

