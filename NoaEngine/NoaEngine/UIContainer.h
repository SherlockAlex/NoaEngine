#ifndef NOAENGINE_UICONTAINER_H
#define NOAENGINE_UICONTAINER_H

#include <string>
#include "Transform.h"
#include "UIBody.h"

namespace noa {
	class UIDocument;
	class UIComponent;

	class UIContainer final:public UIBody
	{
	private:
		friend class UIDocument;
		friend class UIContainer;
		friend class UIComponent;
	private:
		UIContainer(UIDocument* canvas);
		UIContainer(UIContainer* father);
		~UIContainer();
		void Delete(UIContainer*& ptr);
	public:
		static UIContainer* Create(UIDocument* canvas);
		static UIContainer* Create(UIContainer* father);

		UIContainer& SetID(const std::string& id);
		UIContainer& SetLocalPosition(int x, int y);
		UIContainer& SetGlobalPosition(int x, int y);
		UIContainer& SetVisiable(bool value);
		UIContainer* Apply();

		template<class T>
		T* GetElementByID(const std::string& id)
		{

			for (auto& element : uiComponent)
			{
				T* buffer = dynamic_cast<T*>(element);
				if (buffer == nullptr || buffer->id != id)
				{
					continue;
				}
				return buffer;
			}
			//这里面是null

			for (auto& child : subContainers)
			{
				T* buffer = child->GetElementByID<T>(id);
				if (buffer)
				{
					return buffer;
				}
			}
			return nullptr;

		}

		template<>
		noa::UIContainer* GetElementByID(const std::string& id)
		{

			if (this->id == id)
			{
				return this;
			}

			for (auto& element : subContainers)
			{
				if (element == nullptr || element->id != id)
				{
					continue;
				}
				return element;
			}
			return nullptr;
		}


		size_t GetContainerIndex();
	private:
		void Start();
		void Update();
		void Render();

		void DestroyUIContainer();
		void AddUIComponent(UIComponent* component);
		void AddUIContainer(UIContainer* container);
	private:
		bool visiable = false;
		std::vector<UIComponent*> uiComponent;
		std::vector<UIContainer*> subContainers;
		size_t index = 0;
	public:
		std::string id = "container";
	};
}

#endif // !NOAENGINE_UICONTAINER_H
