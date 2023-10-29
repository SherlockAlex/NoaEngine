#include "Item.h"
#include "WolfResource.h"

Item::Item(Scene* scene, Sprite* sprite) :Actor(scene)
{
	rigid->useGravity = false;
	collider->isTrigger = true;
	tag = "item";

	sprite->scale = { 32,32 };
	spriteRenderer->SetSprite(sprite);
	
	
}

Item::~Item()
{
	
}

Item* Item::Create(Scene* scene, Sprite* sprite)
{
	return NObject<Item>::Create<Scene*,Sprite*>(scene, sprite);
}

void Item::SetInteractAudioClip(std::shared_ptr<AudioClip> audioClip)
{
	audioSource->SetAudioClip(audioClip);
}

void Item::OnTrigger(const Collision & other)
{
	if (other.CompareTag("Player"))
	{
		//这个地方有问题
		pickEvent.Invoke();
		audioSource->Play(false);
		Destroy();
		
	}
}

void Item::Update()
{
	this->transform.posZ *= 0.85;
}

void Item::OnDisable()
{
	
}