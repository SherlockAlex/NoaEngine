#pragma once

#include "Collider2D.h"

namespace noa {
	struct Grid;

	class PhysicsSystem
	{
	private:
		Grid grid;

	public:

		void SetGrid(int width,int height);
		void Update();

	private:

		void FindCollisionsGrid();

		void CheckCellsCollisions(Cell& cell1, Cell& cell2);

		bool Collide(CircleCollider2D* obj1, CircleCollider2D* obj2);

		void SolveCollision(CircleCollider2D* obj1, CircleCollider2D* obj2);
	};
}


