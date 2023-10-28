#include "AudioSystem.h"

bool noa::AudioSystem::init = false;

void noa::AudioClip::SetVolume(float value)
{
	this->volume = value;
}
