#include "BackgroundLayer.h"

BackgroundLayer::BackgroundLayer(){}
BackgroundLayer::~BackgroundLayer(){}

void BackgroundLayer::setup(int windowWidth, int windowHeight, int offsetCameratH){

	float sizeW = windowWidth * 0.5f;
	float sizeH = windowHeight * 0.5f;

	// Set Background points
	mBgVertices[0] = Vec3f(-sizeW, -sizeH, -100.0f);
	mBgVertices[1] = Vec3f(-sizeW, sizeH, -100.0f);
	mBgVertices[2] = Vec3f(sizeW, -sizeH, -100.0f);
	mBgVertices[3] = Vec3f(sizeW, sizeH, -100.0f);
	// Set Background initial color
	mBgColors[0] = ColorA(1.0f, 0.0f, 0.0f, 1.0f);
	mBgColors[1] = ColorA(0.0f, 1.0f, 0.0f, 1.0f);
	mBgColors[2] = ColorA(0.0f, 0.0f, 1.0f, 1.0f);
	mBgColors[3] = ColorA(1.0f, 1.0f, 0.0f, 1.0f);
}

// Update Colors Background
void BackgroundLayer::update(float elapsedTime, Color &outColor){

	Color bgColorBottom(CM_HSV,
					(cos((elapsedTime + 30) * 4.0f) + 1) * 0.5f,
					1.0f,
					(sin(elapsedTime * 2.05f) + 1) * 0.4f
	);

	Color bgColorTop = Color(CM_HSV,
		(cos(elapsedTime * 4.0f) + 1) * 0.5f,
		1.0f,
		(sin(elapsedTime * 2.05f) + 1) * 0.8f
	);

	mBgColors[0] = mBgColors[2] = bgColorBottom;
	mBgColors[1] = mBgColors[3] = bgColorTop;


	Color ambiantColor(CM_HSV,
		(cos((elapsedTime) * 4.0f) + 1) * 0.5f,
		1.0f,
		1.0f
		);

	outColor.r = ambiantColor.r;
	outColor.g = ambiantColor.g;
	outColor.b = ambiantColor.b;
}

// Draw Background Quad
void BackgroundLayer::draw(){
	
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, mBgVertices);
	glColorPointer(4, GL_FLOAT, 0, mBgColors);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
