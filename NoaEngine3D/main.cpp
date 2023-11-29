#include "Engine3D.h"
#include "RenderTriangle.h"
#include "RenderRect.h"
#include "RenderColoredRect.h"

int main()
{
	noa::noa3d::Engine3D engine;
	engine.beginAction = RenderTriangle::Begin;
	engine.tickAction = RenderTriangle::Tick;
	engine.Run();
	return 0;
}
