#pragma once
#include "EditorWindow.h"

class UIBody;
// Ù–‘√Ê∞Â
namespace noa {
	class PropertyWindow:public noa::EditorWindow
	{
	private:
		EDITOR(PropertyWindow)
	private:
		PropertyWindow();
		~PropertyWindow();

		void OnGUI() override;

	public:
		noa::Vector<int> GetPositionValue();

	private:
		int position[2] = {0,0};
		UIBody* body = nullptr;

	};
}