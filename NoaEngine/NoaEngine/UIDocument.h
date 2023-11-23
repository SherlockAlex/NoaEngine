#ifndef NOAENGINE_UIDOCUMENT_H
#define NOAENGINE_UIDOCUMENT_H

#include "Actor.h"
#include "ActorComponent.h"

namespace noa {
	class UIContainer;

	class UIDocument
	{
	protected:
		UIDocument();
		virtual ~UIDocument();
	public:

		void SetDocumentID(const std::string& id);

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
		void UIDocumentStart();
		void UIDocumentUpdate();
		void UIDocumentRender();
	private:
		friend class UIContainer;
		friend class UIHub;

		std::stack<UIContainer*> containerStack;
		std::vector<UIContainer*> containerList;

		std::string id = "document";

	};


	class UIDocumentActor final : public Actor, public UIDocument
	{
	private:
		NOBJECT(UIDocumentActor)
	private:
		UIDocumentActor(Scene* scene);
		~UIDocumentActor() override;
	public:
		static UIDocumentActor* Create(Scene* scene);

		void Start() override;
		void Update() override;
		void Render() override;
		UIDocumentActor& SetActorTag(const std::string& tag);
		UIDocumentActor& SetID(const std::string& id);
		UIDocumentActor* Apply();

	};

	class UIDocumentComponent final : public ActorComponent, public UIDocument
	{
	private:
		NOBJECT(UIDocumentComponent)
	private:
		UIDocumentComponent(Actor* actor);
		~UIDocumentComponent() override;
	public:
		static UIDocumentComponent* Create(Actor* actor);

		UIDocumentComponent& SetID(const std::string& id);
		UIDocumentComponent* Apply();

		void Start() override;
		void Update() override;
		void Render() override;

	};
}

#endif // NOAENGINE_UIDOCUMENT_H

