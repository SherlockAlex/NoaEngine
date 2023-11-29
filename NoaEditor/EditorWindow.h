#pragma once
#include "ImGuiHelper.h"
#include "NoaEditor.h"
#include <string>

namespace noa {

	class EditorWindow:public noa::NoaObject
	{
	private:
		friend class Editor;
		NOBJECT(EditorWindow)
	private:
		void Delete(EditorWindow*& ptr);

	protected:
		EditorWindow();
		virtual ~EditorWindow();
		virtual void OnGUI() = 0;

	public:
		void SetName(const std::string& name);

	private:
		std::string name = "Editor Window";
	};
}

#define EDITOR(T) template<class T> friend class noa::NObject;
