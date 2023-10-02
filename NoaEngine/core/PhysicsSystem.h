#pragma once

#include "Collider2D.h"

namespace noa 
{

	class PhysicsSystem
	{
	public:
		static Grid grid;

	public:
		static void SetGrid(int width,int height);
		static void Update();

	private:

		static void FindCollisionsGrid();

		static void CheckCellsCollisions(Cell& cell1, Cell& cell2);

		static bool Collide(CircleCollider2D* obj1, CircleCollider2D* obj2);

		static void SolveCollision(CircleCollider2D* obj1, CircleCollider2D* obj2);
	};
}


