#pragma once
#include "LiveEntity.h"


class Enimy:public LiveEntity
{
	float delay = 0.0f;
	NOBJECT(Enimy)
	Enimy(noa::Scene* scene);
public:
	void TakeDamage(int damage) override;
	void SetPosition(const noa::Vector<float>& position);

	void Update() override;

private:
	noa::Rigidbody* rigid = noa::Rigidbody::Create(this);
	noa::CircleCollider2D* collider = noa::CircleCollider2D::Create(this,rigid);
	noa::SpriteRenderer* spriteRenderer = noa::SpriteRenderer::Create(this);
	

};

