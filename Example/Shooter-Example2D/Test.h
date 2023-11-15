#pragma once
#include "Engine.h"
#include "ResourceManager.h"
class Test:public noa::Actor
{
private:
	ACTOR(Test)
private:
	Test(noa::Scene * scene);
	~Test();

private:

	void Start() override;
	void Update() override;

public:
	noa::SpriteRenderer* spriteRenderer = noa::SpriteRenderer::Create(this)
		->SetSprite(&ResourceManager::circleImage)
		.SetScale(32.0f / 256, 32.0f / 256)
		.Apply();

	noa::Rigidbody* rigid = noa::Rigidbody::Create(this);

	noa::CircleCollider2D* collider = 
		noa::CircleCollider2D::Create(this)
		->SetRigidbody(rigid)
		.SetRadius(0.5f)
		.Apply();

	noa::TileCollider2D* tileCollider = 
		noa::TileCollider2D::Create(this)
		->SetRigidbody(rigid)
		.SetScale(1.0f, 1.0f)
		.Apply();

	noa::TileMapCamera* camera = nullptr;


};

