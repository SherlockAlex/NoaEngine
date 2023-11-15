#pragma once
#include "GameEngine.h"
using namespace noa;
class Bullet:public Actor					
{
public:
	SpriteRenderer* spriteRenderer = SpriteRenderer::Create(this);
	Rigidbody* rigid = Rigidbody::Create(this)
		->SetUseGravity(false)
		.Apply();

	TileCollider2D* tileCollider = TileCollider2D::Create(this)
		->SetRigidbody(rigid)
		.Apply();

	CircleCollider2D* collider = CircleCollider2D::Create(this)
		->SetRigidbody(rigid)
		.Apply();

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

