#pragma once
#include "./../NoaEngine/NoaEngine/NoaEngine.h"

namespace noa 
{
	class NoaEditor:public NoaEngine
	{
	public:
		NoaEditor();
		~NoaEditor();

		void Start() override;
		void BeforeUpdate() override;
		void Update() override;
		void Render() override;
		void OnExit() override;
	};
}


