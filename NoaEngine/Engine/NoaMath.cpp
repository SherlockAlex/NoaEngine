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
        const float result = currentValue - speed * Time::realDeltaTime;
        if (result <= targetValue)
        {
            return targetValue;
        }
        return result;
    }

    return targetValue;
}

noa::Vector<float> noa::Math::FindArithmeticMean(
    const std::vector<noa::Vector<float>>& vertices
) {

    const float count = static_cast<float>(vertices.size());
    if (count <= 0.0f)
    {
        return {};
    }

    float vx = 0.0f;
    float vy = 0.0f;
    for (const auto& v:vertices)
    {
        vx += v.x;
        vy += v.y;
    }

    

    return { vx/ count,vy/ count };

}

bool noa::Math::IntersectCirclePolygon(
    const noa::Vector<float>& circleCenter
    , float radius
    , const std::vector<noa::Vector<float>>& vertices
    , noa::Vector<float>* normalPtr
    , float* depthPtr
)
{
    noa::Vector<float> normal = {};
    float depth = FLT_MAX;

    noa::Vector<float> axis = {};
    float minA, maxA, minB, maxB;
    float axisDepth = 0.0f;
    //分离轴定律检测两个多边形是否重合
    for (int i = 0; i < vertices.size(); i++)
    {
        const noa::Vector<float>& va =
            vertices[i];
        const noa::Vector<float>& vb =
            vertices[(i + 1) % vertices.size()];
        noa::Vector<float> edge = vb - va;
        axis = { -edge.y,edge.x };

        ProjectVertices(vertices, axis, &minA, &maxA);
        ProjectCircle(circleCenter, radius, axis, &minB, &maxB);

        if (minA >= maxB || minB >= maxA)
        {
            return false;
        }

        axisDepth = std::min(maxB - minA, maxA - minB);
        if (axisDepth < depth)
        {
            depth = axisDepth;
            normal = axis;
        }
    }

    const int cpIndex = FindClosesPointOnPolygon(circleCenter,vertices);
    noa::Vector<float> cp = vertices[cpIndex];

    axis = cp - circleCenter;

    ProjectVertices(vertices, axis, &minA, &maxA);
    ProjectCircle(circleCenter, radius, axis, &minB, &maxB);

    if (minA >= maxB || minB >= maxA)
    {
        return false;
    }

    axisDepth = std::min(maxB - minA, maxA - minB);
    if (axisDepth < depth)
    {
        depth = axisDepth;
        normal = axis;
    }

    depth /= normal.Magnitude();
    normal = normal.Normalize();

    const noa::Vector<float> polygonCenter = FindArithmeticMean(vertices);

    const noa::Vector<float> direction = polygonCenter - circleCenter;

    if (noa::Vector<float>::Dot(direction, normal) >= 0.0f)
    {
        normal.x = -normal.x;
        normal.y = -normal.y;
    }

    if (depthPtr != nullptr)
    {
        (*depthPtr) = depth;
    }

    if (normalPtr != nullptr)
    {
        (*normalPtr) = normal;
    }

    return true;
}

bool noa::Math::IntersectPolygons(
    const std::vector<noa::Vector<float>>& verticesA
    , const std::vector<noa::Vector<float>>& verticesB
    ,noa::Vector<float>* normalPtr
    ,float* depthPtr
)
{

    noa::Vector<float> normal = {};
    float depth = FLT_MAX;

    //分离轴定律检测两个多边形是否重合
    for (int i = 0;i<verticesA.size();i++) 
    {
        const noa::Vector<float>& va = 
            verticesA[i];
        const noa::Vector<float>& vb = 
            verticesA[(i + 1) % verticesA.size()];
        noa::Vector<float> edge = vb - va;
        noa::Vector<float> axis = {-edge.y,edge.x};

        float minA, maxA, minB, maxB;
        ProjectVertices(verticesA,axis,&minA,&maxA);
        ProjectVertices(verticesB,axis,&minB,&maxB);

        if (minA>=maxB||minB>=maxA) 
        {
            return false;
        }

        const float axisDepth = std::min(maxB - minA,maxA-minB);
        if (axisDepth<depth)
        {
            depth = axisDepth;
            normal = axis;
        }
    }

    for (int i = 0; i < verticesB.size(); i++)
    {
        const noa::Vector<float>& va =
            verticesB[i];
        const noa::Vector<float>& vb =
            verticesB[(i + 1) % verticesB.size()];
        noa::Vector<float> edge = vb - va;
        noa::Vector<float> axis = { -edge.y,edge.x };

        float minA, maxA, minB, maxB;
        ProjectVertices(verticesA, axis, &minA, &maxA);
        ProjectVertices(verticesB, axis, &minB, &maxB);

        if (minA >= maxB || minB >= maxA)
        {
            return false;
        }

        const float axisDepth = std::min(maxB - minA, maxA - minB);
        if (axisDepth < depth)
        {
            depth = axisDepth;
            normal = axis;
        }

    }

    depth /= normal.Magnitude();
    normal = normal.Normalize();

    const noa::Vector<float> centerA = FindArithmeticMean(verticesA);
    const noa::Vector<float> centerB = FindArithmeticMean(verticesB);

    const noa::Vector<float> direction = centerB - centerA;

    if (noa::Vector<float>::Dot(direction,normal)>=0.0f)
    {
        normal.x = -normal.x;
        normal.y = -normal.y;
    }

    if (depthPtr!=nullptr)
    {
        (*depthPtr) = depth;
    }

    if (normalPtr!=nullptr)
    {
        (*normalPtr) = normal;
    }

    return true;
}

void noa::Math::ProjectVertices(
    const std::vector<noa::Vector<float>>& vertices
    , noa::Vector<float>& axis
    , float* minPtr
    , float* maxPtr) 
{
    float min = FLT_MAX;
    float max = FLT_MIN;

    for (int i = 0;i<vertices.size();i++) 
    {
        noa::Vector<float> v = vertices[i];
        const float proj = noa::Vector<float>::Dot(v,axis);
        
        if (proj<min)
        {
            min = proj;
        }
        if (proj>max) 
        {
            max = proj;
        }

    }

    if (minPtr!=nullptr) 
    {
        (*minPtr) = min;
    }
    if (maxPtr != nullptr)
    {
        (*maxPtr) = max;
    }
}

int noa::Math::FindClosesPointOnPolygon(
    const noa::Vector<float>& circleCenter
    , const std::vector<noa::Vector<float>>& vertices
) {
    int result = -1;
    float minDistance = FLT_MAX;
    for (int i = 0;i<vertices.size();i++)
    {
        noa::Vector<float> v = vertices[i];
        const float distance = noa::Vector<float>::Distance(v, circleCenter);
        
        if (distance<minDistance) 
        {
            minDistance = distance;
            result = i;
        }
    }
    return result;
}

void noa::Math::ProjectCircle(
    const noa::Vector<float>& center
    , float radius
    , const noa::Vector<float>& axis
    , float* minPtr
    , float* maxPtr
) {
    noa::Vector<float> direction = axis.Normalize();
    noa::Vector<float> directionAndRadius = direction * radius;
    noa::Vector<float> p1 = center;
    noa::Vector<float> p2 = center+directionAndRadius*2.0f;

    float min = noa::Vector<float>::Dot(p1, axis);
    float max = noa::Vector<float>::Dot(p2, axis);

    if (min>max) 
    {
        float temp = min;
        min = max;
        max = temp;
    }

    if (minPtr!=nullptr) 
    {
        (*minPtr) = min;
    }

    if (maxPtr != nullptr)
    {
        (*maxPtr) = max;
    }



}