#ifndef NOAENGINE_TEXTURE_H
#define NOAENGINE_TEXTURE_H

#include "Core.h"

namespace noa {

	class NOA_API Texture
	{
	public:
		int width = 0;
		int height = 0;
	protected:
		Texture();
	public:
		virtual ~Texture();

		virtual void UpdateTexture(const void* pixelBuffer, const int width, const int height) = 0;
		virtual void EnableAlpha() = 0;

	public:
		template<class T>
		T* GetTextureAs() {
			return dynamic_cast<T*>(this);
		}

	};
}


#endif


