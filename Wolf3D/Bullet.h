#pragma once
#include "GameEngine.h"
using namespace noa;
class Bullet:public Actor					
{
public:
	SpriteRenderer* spriteRenderer = SpriteRenderer::Create(this);
	Rigidbody* rigid = Rigidbody::Create(this);
	CircleCollider2D* collider = CircleCollider2D::Create(this, rigid);

	Animation* animation = Animation::Create(this, 20, false);
	Vector<float> dir = {0,0};
	float speed = 9;
	

protected:
	Bullet(Scene* scene, Sprite* sprite);
public:
	static Bullet * Create(Scene* scene, Sprite* sprite);
	void Delete() override;

	~Bullet();

	void Update() override;

	void OnHitTile() override;

	void OnTrigger(Collision other) override;

};

