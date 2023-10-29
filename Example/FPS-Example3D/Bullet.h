#pragma once
#include "GameEngine.h"
using namespace noa;
class Bullet:public Actor					
{
public:
	SpriteRenderer* spriteRenderer = SpriteRenderer::Create(this);
	Rigidbody* rigid = Rigidbody::Create(this);
	CircleCollider2D* collider = CircleCollider2D::Create(this, rigid);

	Animation* animation = Animation::Create(this);
	Vector<float> dir = {0,0};
	float speed = 9;
	

protected:
	Bullet(Scene* scene, Sprite* sprite);
	virtual ~Bullet();
public:
	static Bullet * Create(Scene* scene, Sprite* sprite);

	void Update() override;

	void OnHitTile() override;

	void OnTrigger(const Collision & other) override;

};

