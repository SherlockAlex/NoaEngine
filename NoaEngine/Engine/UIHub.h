#ifndef NOAENGINE_UIHUB_H
#define NOAENGINE_UIHUB_H
#include <vector>
#include "Core.h"

namespace noa {
	class NOA_API UIDocument;

	class NOA_API UIHub
	{
	public:
		static UIDocument* GetDocumentByID(const std::string& id);
	private:
		static void RemoveDocument(UIDocument* document);
	private:
		friend class UIDocument;
		static std::vector<UIDocument*> documents;
	};
}

#endif // !NOAENGINE_UIHUB_H
