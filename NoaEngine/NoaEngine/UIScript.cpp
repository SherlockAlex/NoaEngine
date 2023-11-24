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
	this->body->AddScript(this);
}

noa::UIScript::~UIScript() {

}

noa::UIBody* noa::UIScript::GetBody() {
	return this->body;
}