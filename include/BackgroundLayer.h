#pragma once
//////////////////////////////////////////////
// Class BackgroundLayer
// Quad plane
//////////////////////////////////////////////

#include "cinder/Cinder.h"
#include "cinder/Rand.h"
#include "cinder/Rect.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h" 
#include "Resources.h"

#include "Vertice.h"

using namespace ci;
using namespace std;

////////////////////////////////////
// Class BackgroundLayer Definition
class BackgroundLayer
{
private:
	//Vertice mBackground[4];
	Vec3f mBgVertices[4];
	ColorA mBgColors[4];

public:
	BackgroundLayer();
	~BackgroundLayer();
	void setup(int windowWidth, int windowHeight, int offsetCameratH = 0);
	void update(float elapsedTime, Color &outColor);
	void draw();
};
