#pragma once
#include "GameEngine.h"

class Bullet :public noa::Actor
{
public:
	NOBJECT(Bullet)
private:
	Bullet(noa::Scene* scene);
public:

public:

	void Start() override;

	void Update() override;

	void SetPostion(const noa::Vector<float>& pos);

private:
	noa::Rigidbody* rigid = noa::Rigidbody::Create(this);
	noa::SpriteRenderer* spriteRenderer = noa::SpriteRenderer::Create(this);
	noa::Sprite sprite = noa::Sprite(noa::resource.LoadSprFile("./Assets/Fly/bullet.spr"), { noa::pixelWidth / 10,noa::pixelWidth / 10 });
	float time = 0;
};

