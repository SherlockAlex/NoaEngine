#pragma once
#include "Engine.h"
class Bullet:public noa::Actor
{
private:
	NOBJECT(Bullet)
private:
	Bullet(noa::Scene * scene);
	~Bullet();

	noa::Vector<float> direction;

private:
	
	void Start() override;
	void OnEnable() override;
	void Update() override;
	void OnDisable() override;
	
	void OnHitTile() override;

public:
	void SetDirection(float x,float y);
private:

	noa::Rigidbody* rigid = noa::Rigidbody::Create(this);
	noa::SpriteRenderer* spriteRenderer = noa::SpriteRenderer::Create(this);
	noa::CircleCollider2D* collider = noa::CircleCollider2D::Create(this,rigid);

	float timer = 0;

	float speed = 50;
};

