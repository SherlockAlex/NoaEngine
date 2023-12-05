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
		->SetUseGravity(false)
		.SetBodyType(noa::BodyType::DYNAMIC)
		.SetBounce(1.0f)
		.Apply();

	noa::BoxCollider2D* boxCollider2D =
		noa::BoxCollider2D::Create(this)
		->SetRigidbody(rigid)
		.SetSize(0.5f, 0.5f)
		.Apply();

public:
	noa::Camera2D* camera = nullptr;
	bool isHandled = false;

	noa::TileCollider2D* tileCollider2D =
		noa::TileCollider2D::Create(this)
		->SetRigidbody(rigid)
		.SetSize(0.5f, 0.5f)
		.Apply();

};

