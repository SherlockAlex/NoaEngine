#include "LiveEntity.h"

void LiveEntity::TakeDamage(int damage)
{
	hp -= damage;
	if (hp<=0)
	{
		death.Invoke();
	}
}
