#include "Prop.h"
#include "GameLayer.h"

void Prop::killSelf(float)
{
	GameLayer* g = (GameLayer*)getParent();
	g->_props->removeObject(this);
	removeFromParent();
}
