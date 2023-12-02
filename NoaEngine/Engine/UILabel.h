#ifndef NOAENGINE_LABEL_H
#define NOAENGINE_LABEL_H

#include "Graphic.h"
#include "UIComponent.h"
#include "UIContainer.h"
#include "Core.h"

namespace noa {
	class NOA_API Label :public UIComponent
	{
	private:
		Label(UIContainer* group);
		~Label();

		void Start() override;
		void Update() override;
		void Render() override;

	public:

		static Label* Create(UIContainer* group);

		Label& SetID(const std::string& id);
		Label& SetAnchor(float x, float y);
		Label& SetColor(uint32_t color);
		Label& SetActive(bool value);
		Label& SetFontSize(uint32_t size);
		Label& SetLocalPosition(int x, int y);
		Label& SetGlobalPosition(int x,int y);
		Label& SetText(const std::wstring& text);
		Label& SetRowCount(int count);
		Label* Apply();

	public:
		std::wstring text = L"text";
		uint32_t color = BLACK;
		uint32_t size = 25;

	private:
		int rowCount = -1;
		std::wstring textBuffer = L"";
	};
}

#endif // !NOAENGINE_LABEL_H

