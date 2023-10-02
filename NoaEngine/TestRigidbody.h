#pragma once
#include "./core/NoaEngine.h"

class Collider2D
{
public:
	noa::Rigidbody* rigidbody = nullptr;
};

class CircleCollider2D :public Collider2D
{
public:
	float radius = 1;
};

extern std::vector<CircleCollider2D*> physicsWorld;

typedef struct Cell {
	std::vector<CircleCollider2D*> colliders;
}Cell;

typedef struct Grid 
{
	int width;
	int height;

	std::vector<Cell> cells;

	//返回坐标为x,y的cell
	Cell & GetCell(int x, int y) 
	{
		return cells[y * width + x];
	}

}Grid;

class PhysicsManager
{
	Grid grid;

	void Update(float deltaTime);

	void UpdateSubSteps(float deltaTime,int subSteps);

	void FindCollisions();

	void FindCollisionsGrid();

	void CheckCellsCollisions(Cell & cell1,Cell & cell2);

	bool Collide(CircleCollider2D* obj1, CircleCollider2D* obj2);

	void SolveCollision(CircleCollider2D* obj1, CircleCollider2D* obj2);



};

