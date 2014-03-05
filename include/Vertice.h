#pragma once

//////////////////////////////////////////////
// Structure Vertice
//////////////////////////////////////////////

#include "cinder/Cinder.h"
#include "cinder/Color.h"

using namespace ci;

////////////////////////////////////
// Vertice Definition
struct Vertice
{
	Vec3f position;
	Vec3f normal;
	ColorA color;
	int neighboursId[4];
	Vec3f neighboursNormals[4];
};
