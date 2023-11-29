#include "ActorMotion.h"
#include "Rigidbody.h"
#include "Debug.h"


noa::ActorMotion::ActorMotion(noa::Actor* actor):ActorComponent(actor) 
{
	motions.reserve(100);
	this->actorHandle = actor;
}

noa::ActorMotion::~ActorMotion() {

}

noa::ActorMotion* noa::ActorMotion::Create(noa::Actor* actor) 
{
	if (!actor) 
	{
		noa::Debug::Error("Create ActorMotion component failed,please give a Actor object");
	}

	return noa::NObject<noa::ActorMotion>::Create(actor);

}

noa::ActorMotion& noa::ActorMotion::AddMotion(
	const noa::Vector<float>& destination
	,float speed
) 
{
	Motion motion;
	motion.destination = destination;
	motion.speed = speed;
	this->motions.push_back(motion);
	this->motionCount = this->motions.size();
	return *this;
}

noa::ActorMotion& noa::ActorMotion::SetCallback(
	size_t index
	,std::function<void()> action
)
{
	if (motionCount == 0||index>=motionCount) 
	{
		return *this;
	}
	motions[index].finishedEvent += action;

	return *this;
}

noa::ActorMotion& noa::ActorMotion::SetPrecision(float precision) 
{
	this->precision = precision;
	return *this;
}

noa::ActorMotion& noa::ActorMotion::SetLoop(bool value)
{
	this->loop = value;
	return *this;
}

noa::ActorMotion& noa::ActorMotion::Act() {
	this->finished = false;
	return *this;
}

noa::ActorMotion& noa::ActorMotion::Stop() {
	this->finished = true;
	return *this;
}

noa::ActorMotion& noa::ActorMotion::SetRigidbody(noa::Rigidbody* rigid) 
{
	this->rigidbodyHandle = rigid;
	return *this;
}

noa::ActorMotion* noa::ActorMotion::Apply() {
	return this;
}

void noa::ActorMotion::Start() {

}

void noa::ActorMotion::Update() {
	if (!rigidbodyHandle) 
	{
		noa::Debug::Log("ActorMotion's handled rigidbody is empty");
		return;
	}

	if (finished||motions.empty()) 
	{
		return;
	}

	const Vector<float>& deltaPosition = motions[currentMotionIndex].destination
		- this->actorHandle->transform.position;
	const float speed = motions[currentMotionIndex].speed;
	if (deltaPosition.SqrMagnitude()<precision)
	{
		//到达了目标点
		motions[currentMotionIndex].finishedEvent();
		currentMotionIndex = currentMotionIndex + 1;
		if (loop) 
		{
			currentMotionIndex = (currentMotionIndex) % motionCount;
		}
		else if(currentMotionIndex>= motionCount)
		{
			currentMotionIndex = 0;
			finished = true;
			this->rigidbodyHandle->velocity = {};
			return;
		}
		
	}

	const Vector<float>& direction = deltaPosition.Normalize();
	const Vector<float>& velocity = direction * speed;
	this->rigidbodyHandle->velocity = velocity;

}