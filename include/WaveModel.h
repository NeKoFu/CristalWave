#pragma once
//////////////////////////////////////////////
// Class WaveModel
// Triangle Strip plane
//////////////////////////////////////////////

#include "cinder/Cinder.h"
#include "cinder/Rand.h"
#include "cinder/Rect.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h" 
#include "cinder/Perlin.h" 
#include "cinder/MayaCamUI.h" 
#include "cinder/CinderMath.h" 
#include "cinder/gl/GlslProg.h"
#include "Resources.h"

#include "Vertice.h"

//#define SHOW_WAVE_TRACE

using namespace ci;
using namespace std;

////////////////////////////////////
// Window Size
struct WindowSize{
	int width;
	int height;
	int halfWidth;
	int halfHeight;
};

////////////////////////////////////
// Class WaveModel Definition
class WaveModel
{
private:
	WindowSize mWindow;
	float mGap;
	float mTime;
	int mNumRows;
	int mNumLines;
	int mNbPoints;
	int mNbIndexes;
	int mWaveMotionLimit;
	int mOffsetH;
	unsigned int mFrameCounter;

	Vec3f * mpVertices;
	Vec3f * mpNormals;
	ColorA* mpColors;
	Vertice* mpWave;
	unsigned short* mpVerticeIndexes;

	Perlin mPerlin;

	string mTrace;

public:
	WaveModel();
	~WaveModel();
	void setup(int windowWidth, int windowHeight, int numRows = 5, int numLines = 200, int offsetCameratH = 0);
	void update(float elapsedTime, float speedFactor = 0);
	void draw();
	void writeInfo(Vec2f position);

private:
	void computePositions(float elapsedTime, float speed = 3.8f);
	void computeNormals();
};

