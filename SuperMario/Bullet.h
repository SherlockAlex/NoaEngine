#pragma once
#include "GameEngine.h"

class Bullet :public noa::Actor
{
public:
	NOBJECT(Bullet)
private:
	noa::Vector<float> dir = {0,-1};
	float speed = 10.0f;
	Bullet(noa::Scene* scene);
public:
	std::string ownTag = "Default";
public:

	void SetDirection(const noa::Vector<float> & direction);

	void SetDestroy();

	void Update() override;

	void SetPostion(const noa::Vector<float>& pos);

	void OnTrigger(const noa::Collision & other) override;

private:
	noa::Rigidbody* rigid = noa::Rigidbody::Create(this);
	noa::CircleCollider2D* collider = noa::CircleCollider2D::Create(this,rigid);
	noa::SpriteRenderer* spriteRenderer = noa::SpriteRenderer::Create(this);
	noa::Sprite sprite = noa::Sprite(noa::resource.LoadSprFile("./Assets/Fly/bullet.spr"), { noa::Screen::width / 10,noa::Screen::width/ 10 });
	float time = 0;
};

