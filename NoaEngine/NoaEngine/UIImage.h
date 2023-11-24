#ifndef NOAENGINE_IMAGE_H
#define NOAENGINE_IMAGE_H

#include "Graphic.h"
#include "UIContainer.h"
#include "UIComponent.h"

#include "Sprite.h"

namespace noa {

	enum class ImageStyle
	{
		DEFAULT,
		COVER
	};

	class Image :public UIComponent {
	private:
		Image(UIContainer* group);
		~Image();

		void Start() override;
		void Update() override;
		void Render() override;

	public:

		static Image* Create(UIContainer* group);

		Image& SetID(const std::string& id);
		Image& SetLocalPosition(int x, int y);
		Image& SetGlobalPosition(int x,int y);
		Image& SetAnchor(float x, float y);
		Image& SetActive(bool value);
		Image& SetSize(int x, int y);
		Image& SetRadius(int radius);
		Image& SetStyle(ImageStyle style);
		Image& SetSprite(Sprite* sprite);
		Image& SetColor(uint32_t color);
		Image* Apply();

	public:
		ImageStyle style = ImageStyle::DEFAULT;
		uint32_t color = WHITE;
		bool isFilpX = false;
	private:
		Sprite sprite;
		Sprite* mapSprite = nullptr;
		std::shared_ptr<SpriteGPU> spriteGPU = nullptr;
		int radius = 0;
	};
}

#endif // !NOAENGINE_IMAGE_H
