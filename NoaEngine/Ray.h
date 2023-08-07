#pragma once

#include "NoaMath.h"

typedef struct Ray {
	//返回射线碰撞到的信息
	float angle = 0.0f;
	float distance = 0.0f;
	bool isHitDoor = false;
	bool isHitWall = false;

	//返回贴图信息
	Vector simple;

}Ray;

