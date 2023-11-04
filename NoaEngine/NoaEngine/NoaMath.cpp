#include "NoaMath.h"
#include "Time.h"

const float noa::Math::Pi = PI;
const float noa::Math::halfPi = HALFPI;
const float noa::Math::doublePi = MUTIPI;

float noa::Math::LinearLerp(float currentValue, float targetValue, float stepSpeed)
{
	//���ȼ���
	return currentValue + (targetValue - currentValue) * stepSpeed * Time::deltaTime;
}
