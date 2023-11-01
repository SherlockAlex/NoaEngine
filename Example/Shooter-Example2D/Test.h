#pragma once
#include "Engine.h"
class Test:public noa::Actor
{
private:
	NOBJECT(Test)
private:
	Test(noa::Scene * scene);
	~Test();

private:

	void Start() override;
	void Update() override;

public:
	noa::SpriteRenderer* spriteRenderer = noa::SpriteRenderer::Create(this);
	noa::Rigidbody* rigid = noa::Rigidbody::Create(this);
	noa::CircleCollider2D* collider = noa::CircleCollider2D::Create(this, rigid);
	noa::TileCollider2D* tileCollider = noa::TileCollider2D::Create(this, rigid);
	noa::TileMapCamera* camera = nullptr;


};

