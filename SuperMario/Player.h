#pragma once
#include "GameEngine.h"
class Player :public noa::Actor {
	NOBJECT(Player)
private:
	Player(noa::Scene* scene);

public:

	void Update() override;

public:
	noa::SpriteRenderer* spriteRenderer = noa::SpriteRenderer::Create(this);
	noa::Rigidbody* rigid = noa::Rigidbody::Create(this);
	noa::CircleCollider2D* collider = noa::CircleCollider2D::Create(this, rigid);

private:
	noa::Sprite sprite = noa::Sprite(noa::resource.LoadSprFile("./Assets/Fly/player.spr"), { noa::pixelWidth / 10,noa::pixelWidth / 10 });
	float delay = 0;

	noa::Audio shootAFX = noa::Audio("./Assets/Fly/Audio/shoot.mp3",noa::AudioType::CHUNK);
};

