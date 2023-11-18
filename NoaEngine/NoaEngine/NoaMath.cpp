#include "NoaMath.h"
#include "Time.h"

const float noa::Math::Pi = PI;
const float noa::Math::halfPi = HALFPI;
const float noa::Math::doublePi = MUTIPI;

float noa::Math::LinearLerp(float currentValue, float targetValue, float stepSpeed)
{
    // 首先计算差值
    float difference = targetValue - currentValue;

    // 根据步长速度和时间间隔进行线性插值
    float interpolatedValue = currentValue + difference * stepSpeed;

    return interpolatedValue;
}
