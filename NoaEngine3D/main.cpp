#include "Engine3D.h"
#include "RenderTriangle.h"
#include "RenderRect.h"
#include "RenderColoredRect.h"

int main()
{
	noa::noa3d::Engine3D engine;
	engine.beginAction = RenderColoredRect::Begin;
	engine.tickAction = RenderColoredRect::Tick;
	engine.Run();
	return 0;
}
