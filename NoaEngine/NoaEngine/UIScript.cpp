#include "UIScript.h"
#include "UIBody.h"

void noa::UIScript::Delete(noa::UIScript*& ptr)
{
	ptr = nullptr;
	delete this;
}

noa::UIScript::UIScript(noa::UIBody* body) 
{
	this->body = body;
}

noa::UIScript::~UIScript() {

}

noa::UIScript* noa::UIScript::Create(noa::UIBody* body) 
{
	if (body) 
	{
		return nullptr;
	}
	return new UIScript(body);
}

noa::UIBody* noa::UIScript::GetBody() {
	return this->body;
}