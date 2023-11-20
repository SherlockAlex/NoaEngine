#include "NoaMath.h"
#include "Time.h"

const float noa::Math::pi = PI;
const float noa::Math::halfPi = HALFPI;
const float noa::Math::doublePi = MUTIPI;

float noa::Math::LinearLerp(float currentValue, float targetValue, float speed)
{

    if (currentValue<targetValue) 
    {
        const float result = currentValue + speed * Time::deltaTime;
        if (result >= targetValue)
        {
            return targetValue;
        }
        return result;
    }
    else if (currentValue>targetValue) 
    {
        const float result = currentValue - speed * Time::deltaTime;
        if (result <= targetValue)
        {
            return targetValue;
        }
        return result;
    }

    return targetValue;
}
