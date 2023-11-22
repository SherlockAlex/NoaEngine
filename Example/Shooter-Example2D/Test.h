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

	noa::Rigidbody* rigid = noa::Rigidbody::Create(this)
		->SetBounce(0.5f)
		.SetUseGravity(true)
		.SetGravityScale(5.5f)
		.SetDamping(0.0f)
		.SetBodyType(noa::BodyType::DYNAMIC)
		.Apply();

	/*noa::ActorMotion* motion = noa::ActorMotion::Create(this)
		->SetRigidbody(rigid)
		.AddMotion({ 5,3 },10)
		.AddMotion({ 2,3 },10)
		.AddMotion({ 10,3 },10)
		.AddMotion({ 2,3 },10)
		.AddMotion({ 2,10 },10)
		.AddMotion({ 2,3 },10)
		.SetCallback(5, []() {noa::Debug::Log("finish this motion"); })
		.SetLoop(true)
		.Act()
		.Apply();*/

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

	/*noa::TileMapCamera* camera = nullptr;*/


};

