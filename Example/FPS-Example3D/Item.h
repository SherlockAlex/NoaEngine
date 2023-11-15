#pragma once
#include "GameEngine.h"
using namespace noa;
class Item :public Actor
{
private:
	NOBJECT(Item)
public:
	NoaEvent<void> pickEvent;

	SpriteRenderer* spriteRenderer = SpriteRenderer::Create(this);

	Rigidbody* rigid = Rigidbody::Create(this);
	Collider2D* collider = CircleCollider2D::Create(this)
		->SetRigidbody(rigid)
		.Apply();

	AudioSource* audioSource = AudioSource::Create(this);
private:
	Item(Scene * scene,Sprite* sprite);
	virtual ~Item();
public:
	static Item* Create(Scene* scene, Sprite* sprite);

	void SetInteractAudioClip(std::shared_ptr<AudioClip> audioClip);

	void OnTrigger(const Collision & other) override;
	void Update() override;
	void OnDisable() override;
};

