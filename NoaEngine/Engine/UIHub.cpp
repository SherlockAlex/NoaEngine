#include <string>

#include "UIHub.h"
#include "UIDocument.h"

std::vector<noa::UIDocument*> noa::UIHub::documents;

noa::UIDocument* noa::UIHub::GetDocumentByID(
	const std::string& id)
{
	for (auto& document : documents)
	{
		if (document && document->id == id)
		{
			return document;
		}
	}
	return nullptr;
}

void noa::UIHub::RemoveDocument(noa::UIDocument* document) {
	
	auto it = std::find(
		documents.begin()
		, documents.end()
		, document);
	if (it == documents.end())
	{
		return;
	}
	documents.erase(it);

}