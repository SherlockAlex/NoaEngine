#ifndef NOAENGINE_NOAMATH_H
#define NOAENGINE_NOAMATH_H

// Math Constance
#define PI 3.14159

//¶¨Òå2Î¬Ê¸Á¿
typedef struct Vector {
	float x = 0.0f;
	float y = 0.0f;

	Vector() {

	}

	Vector(float x,float y) {
		this->x = x;
		this->y = y;
	}

}Vector;

typedef struct nVector {
	int x = 0;
	int y = 0;
}nVector;

#endif

