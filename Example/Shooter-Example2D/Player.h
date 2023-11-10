#pragma once
#include "Engine.h"
#include "ResourceManager.h"
class Player:public noa::Actor
{
private:
	ACTOR(Player)
private:
	Player(noa::Scene * scene);
	~Player();
public:
	void Start() override;
	void Update() override;
public:
	noa::SpriteRenderer* spriteRenderer = noa::SpriteRenderer::Create(this)
		->SetSprite(&ResourceManager::playerImage)
		.SetScale(2, 2)
		.Apply();

	noa::Rigidbody* rigid = noa::Rigidbody::Create(this)
		->SetUseGravity(false)
		.SetDamping(0.0f)
		.SetGravityWeight(5.5f)
		.SetBodyType(noa::BodyType::DYNAMIC)
		.Apply();

	noa::CircleCollider2D* collider =
		noa::CircleCollider2D::Create(this)
		->SetRigidbody(rigid)
		.SetRadius(1.0f)
		.Apply();

	noa::TileCollider2D* tileCollider = 
		noa::TileCollider2D::Create(this)
		->SetRigidbody(rigid)
		.SetScale(1.5f, 1.7f)
		.Apply();

	noa::TileMapCamera* camera = nullptr;
};

