#include <memory>

#include "Renderer.h"
#include "UILabel.h"

extern std::shared_ptr<noa::Renderer> noa::renderer;

noa::Label::Label(UIContainer* group) :UIComponent(group)
{

}

noa::Label::~Label()
{

}

noa::Label* noa::Label::Create(UIContainer* group)
{
	Label* text = new Label(group);
	return text;
}

noa::Label& noa::Label::SetID(const std::string& id)
{
	this->id = id;
	return *this;
}

noa::Label& noa::Label::SetAnchor(float x, float y)
{
	this->anchor.x = x;
	this->anchor.y = y;
	return *this;
}

noa::Label& noa::Label::SetColor(uint32_t color)
{
	this->color = color;
	return *this;
}

noa::Label& noa::Label::SetActive(bool value)
{
	UIComponent::SetActiveInContainer(value);
	return *this;
}

noa::Label& noa::Label::SetFontSize(uint32_t size)
{
	this->size = size;
	this->transform.size = renderer->GetLabelScale(this->text, this->size);
	return *this;
}

noa::Label& noa::Label::SetPosition(int x, int y)
{
	this->transform.position.x = x;
	this->transform.position.y = y;
	return *this;
}

noa::Label& noa::Label::SetText(const std::wstring& text)
{
	this->text = text;
	this->transform.size = renderer->GetLabelScale(this->text, this->size);
	return *this;
}

noa::Label& noa::Label::SetRowCount(int count)
{
	this->rowCount = count;
	return *this;
}

noa::Label* noa::Label::Apply()
{
	return this;
}


void noa::Label::Start()
{

}

void noa::Label::Update()
{

}

void noa::Label::Render() {
	//显示文字

	//下面代码只是暂时的，需要优化

	std::wstring textBuffer = text;
	if (rowCount > 0)
	{
		textBuffer = L"";
		int count = 0;
		const size_t length = text.length();
		for (int i = 0; i < length; i++)
		{
			textBuffer += text[i];
			count++;
			if (count >= rowCount)
			{
				count = 0;
				textBuffer += L"\n";
			}
		}
	}

	this->transform.size = renderer->GetLabelScale(textBuffer, this->size);
	globalTransform.position.x = static_cast<int>(fatherTransform.position.x + transform.position.x - anchor.x * transform.size.x);
	globalTransform.position.y = static_cast<int>(fatherTransform.position.y + transform.position.y - anchor.y * transform.size.y);
	renderer->DrawString(
		textBuffer
		, globalTransform.position.x
		, globalTransform.position.y
		, color
		, size
	);
}
