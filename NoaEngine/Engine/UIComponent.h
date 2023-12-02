#ifndef NOAENGINE_UICOMPONENT_H
#define NOAENGINE_UICOMPONENT_H

#include "UIBody.h"
#include "Core.h"

namespace noa {

	class NOA_API UIContainer;

	class NOA_API UIComponent:public noa::UIBody
	{
	protected:
		UIComponent(UIContainer* canvas);
		virtual ~UIComponent();

	private:
		void Delete(UIComponent*& ptr);

	protected:

		virtual void Start() {};
		virtual void Update() {};		//����UI�Ľ����߼�
		virtual void Render() {};		//����UI�Ļ���

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

