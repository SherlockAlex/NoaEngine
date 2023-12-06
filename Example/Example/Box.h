#pragma once
#include "NoaEngine.h"
class Box:public noa::Actor
{
private:
	ACTOR(Box)
private:
	Box(noa::Scene* scene);
	~Box();

	void Update() override;

	void OnMouseStay() override;

private:
	noa::SpriteRenderer* spriteRenderer 
		= noa::SpriteRenderer::Create(this)
		->SetScale(1.0f,1.0f)
		.Apply();

	noa::Rigidbody* rigid =
		noa::Rigidbody::Create(this)
		->SetUseGravity(true)
		.SetBodyType(noa::BodyType::DYNAMIC)
		.SetBounce(0.5f)
		.Apply();

	noa::BoxCollider2D* boxCollider2D =
		noa::BoxCollider2D::Create(this)
		->SetRigidbody(rigid)
		.SetSize(0.5f, 0.5f)
		.Apply();

	/*noa::CircleCollider2D* boxCollider2D =
		noa::CircleCollider2D::Create(this)
		->SetRigidbody(rigid)
		.SetRadius(0.25f)
		.Apply();*/


public:
	noa::Camera2D* camera = nullptr;
	bool isHandled = false;

	noa::TileCollider2D* tileCollider2D =
		noa::TileCollider2D::Create(this)
		->SetRigidbody(rigid)
		.SetSize(0.5f, 0.5f)
		.Apply();

};

