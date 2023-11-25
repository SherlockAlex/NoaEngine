#ifndef NOAENGINE_UIDOCUMENT_H
#define NOAENGINE_UIDOCUMENT_H

#include "Actor.h"
#include "ActorComponent.h"

namespace noa {
	class UIContainer;

	class UIDocument: public ActorComponent
	{
	private:
		ACTOR_COMPONENT(UIDocumentComponent)
	protected:
		UIDocument(noa::Actor* actor);
		virtual ~UIDocument();
	public:

		static UIDocument* Create(noa::Actor* actor);

		UIDocument& SetID(const std::string& id);
		UIDocument* Apply();

		void Display(size_t index);
		void Display(const std::string& id);
		void Display(UIContainer* container);
		void Close();

		template<class T>
		T* GetElementByID(const std::string& id)
		{
			for (auto& container : containerList)
			{
				if (!container)
				{
					continue;
				}

				T* temp = container->GetElementByID<T>(id);
				if (temp)
				{
					return temp;
				}
			}
			return nullptr;
		}

	private:
		void AddUIContainer(UIContainer* container);
	protected:
		void Start() override;
		void Update() override;
		void Render() override;
	private:
		friend class UIContainer;
		friend class UIHub;

		std::stack<UIContainer*> containerStack;
		std::vector<UIContainer*> containerList;

		std::string id = "document";

	};

}

#endif // NOAENGINE_UIDOCUMENT_H

