#pragma once

#include "Particule.h"

using namespace ci;
using namespace std;

class SparkleParticule : public Particule
{
	unsigned short _nbRepeatDraw;

public:
	SparkleParticule(const BOX playBox, float radius, float mass, float drag);
	void draw();

};