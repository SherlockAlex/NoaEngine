#pragma once
#include "EngineHelper.h"
namespace noa {
	class NoaEditor :public noa::EnginExtension
	{
		void Start() override;
		void BeforeUpdate() override;
		void Update() override;
		void Render() override;
		void OnExit() override;

	public:
		void* eventPtr = nullptr;

	};
}