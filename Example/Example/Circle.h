#pragma once
#include "NoaEngine.h"
class Circle:public noa::Actor
{
private:
	ACTOR(Circle)
private:
	Circle(noa::Scene* scene);
	~Circle();

	void Update() override;

	void OnMouseStay() override;

private:
	noa::SpriteRenderer* spriteRenderer
		= noa::SpriteRenderer::Create(this)
		->SetScale(1.0f, 1.0f)
		.Apply();

	noa::Rigidbody* rigid =
		noa::Rigidbody::Create(this)
		->SetUseGravity(false)
		.SetBodyType(noa::BodyType::DYNAMIC)
		.SetBounce(0.5f)
		.SetMass(10)
		.Apply();

	noa::CircleCollider2D* boxCollider2D =
		noa::CircleCollider2D::Create(this)
		->SetRigidbody(rigid)
		.SetRadius(0.5f)
		.Apply();


public:

	noa::TileCollider2D* tileCollider2D =
		noa::TileCollider2D::Create(this)
		->SetRigidbody(rigid)
		.SetSize(1.0f, 1.0f)
		.Apply();
};

