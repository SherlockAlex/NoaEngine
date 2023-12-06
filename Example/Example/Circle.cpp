#include "Circle.h"

Circle::Circle(noa::Scene* scene) :Actor(scene)
{
	noa::Sprite sprite("circle.spr", 32, 32);
	this->spriteRenderer->SetSprite(&sprite);
}

Circle::~Circle()
{

}

void Circle::Update()
{


}

void Circle::OnMouseStay() {

}