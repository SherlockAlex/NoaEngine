#ifndef NOAENGINE_ENGINEEXTENSION_H
#define NOAENGINE_ENGINEEXTENSION_H

#include "Core.h"

namespace noa {
	// ”Œœ∑“˝«Ê¿©’π
	class NOA_API EnginExtension
	{
	private:
		friend class NoaEngine;
	protected:
		virtual void Start() {};
		virtual void BeforeUpdate() {};
		virtual void Update() {};
		virtual void Render() {};
		virtual void OnExit() {};

	private:
		void Delete(EnginExtension*& ptr);

	};
}

#endif // !NOAENGINE_ENGINEEXTENSION_H

