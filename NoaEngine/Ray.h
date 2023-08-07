#pragma once

#include "NoaMath.h"

typedef struct Ray {
	//����������ײ������Ϣ
	float angle = 0.0f;
	float distance = 0.0f;
	bool isHitDoor = false;
	bool isHitWall = false;

	//������ͼ��Ϣ
	Vector simple;

}Ray;

