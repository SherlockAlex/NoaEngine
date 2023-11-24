#ifndef NOAENGINE_UICOMPONENT_H
#define NOAENGINE_UICOMPONENT_H

#include "UIBody.h"

namespace noa {

	class UIContainer;

	class UIComponent:public noa::UIBody
	{
	protected:
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

	protected:
		bool active = false;
		friend class UIContainer;
		friend class UIDocumentComponent;

	public:
		std::string id = "ui_component";
	};
}

#endif // !NOAENGINE_UICOMPONENT_H

