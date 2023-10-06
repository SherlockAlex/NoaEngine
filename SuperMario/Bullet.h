#pragma once
#include "GameEngine.h"

class Bullet :public noa::Actor
{
public:
	NOBJECT(Bullet)
private:
	Bullet(noa::Scene* scene);
public:
	std::string ownTag = "Default";
public:

	void SetDestroy();

	void Update() override;

	void SetPostion(const noa::Vector<float>& pos);

	void OnTrigger(const noa::Collision & other) override;

private:
	noa::Rigidbody* rigid = noa::Rigidbody::Create(this);
	noa::CircleCollider2D* collider = noa::CircleCollider2D::Create(this,rigid);
	noa::SpriteRenderer* spriteRenderer = noa::SpriteRenderer::Create(this);
	noa::Sprite sprite = noa::Sprite(noa::resource.LoadSprFile("./Assets/Fly/bullet.spr"), { noa::pixelWidth / 10,noa::pixelWidth / 10 });
	float time = 0;
};

