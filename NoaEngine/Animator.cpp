#include "NoaEngine.h"

Animator::Animator()
{
	printf("Init Animator\n");
}

Animator::Animator(Uint32* frameImage)
{
	framesImage.push_back(frameImage);
	currentFrame = frameImage;
	printf("Init Animator\n");
}

Animator::~Animator()
{

}

Uint32* Animator::GetCurrentFrameImage() {
	return currentFrame;
}

Uint32* Animator::GetFrameImage(int frame) {
	if (framesImage.empty())
	{
		//printf("return frame image");
		return nullptr;
	}
	if (frame >= framesImage.size())
	{
		//printf("return frame image");
		return framesImage[0];
	}
	//printf("return frame image");
	return framesImage[frame];
}

void Animator::Play(int frame) {
	currentFrame = GetFrameImage(frame);
}

void Animator::InsertFrameImage(Uint32* frameImage)
{
	printf("Insert Animator\n");
	framesImage.push_back(frameImage);
	if (currentFrame == nullptr)
	{
		currentFrame = frameImage;
	}
}

Uint32 Animator::GetCurrentFramePixel(int index)
{
	if (index < 0 || currentFrame == nullptr)
	{
		return BLACK;
	}
	return currentFrame[index];
}
