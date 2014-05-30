#include "cinder/Cinder.h"
#include "cinder/Rand.h"
#include "cinder/Rect.h"
#include "cinder/app/AppBasic.h"
#include "cinder/app/AppScreenSaver.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h" 
#include "cinder/Perlin.h" 
#include "cinder/MayaCamUI.h" 
#include "cinder/CinderMath.h" 
#include "cinder/gl/GlslProg.h"

#include "Resources.h"
#include "ShaderFresnel.h"

#include "BackgroundLayer.h"
#include "WaveModel.h"

#define SCREENSAVER_MODE
#ifdef SCREENSAVER_MODE
#define APP_CLASS_TYPE AppScreenSaver
#else
#define APP_CLASS_TYPE AppBasic
#endif

using namespace ci;
using namespace ci::app;
using namespace std;

class CristalWaveApp : public APP_CLASS_TYPE {
  public:
	virtual void prepareSettings(Settings *settings);
	virtual void setup();
	virtual void update();
	virtual void draw();

private:
	void fadeLayer(float opacity);
	void setCameraOrtho(Vec3f eye, bool reverse = false);
	
  protected:
	  float mOpacity;
	  float mOffsetTime;
	  int mOffsetCameratH;

	  WaveModel mWave;
	  BackgroundLayer mBackground;

	  CameraOrtho mCamera;

	  Vec3f mCenter;

	  // Shader parameters
	  AShader * mpWaveShader;
	  ShaderFresnel mFresnelShader;
};

//////////////////////////////////////////////
// Prepare Window settings for Cinder application
void CristalWaveApp::prepareSettings(Settings *settings){
	APP_CLASS_TYPE::prepareSettings(settings);
#ifdef SCREENSAVER_MODE
	settings->enableSecondaryDisplayBlanking(false);
#endif
	settings->setFrameRate(60.0f);
	//settings->disableFrameRate();
}

void CristalWaveApp::setup()
{
	float sizeW = getWindowWidth() * 0.5f;
	float sizeH = getWindowHeight() * 0.5f;
	float	x = 0.0f,
			z = 0.0f,
			y = 0.0f;

	/////////////////////////////////////////////////
	int numRows = 64;
	int gap = 7 + getWindowWidth() / 2000;
	int numLines = getWindowWidth() / gap + 1;
	/////////////////////////////////////////////////

	mOpacity = 0.0f;
	mOffsetCameratH = 60; // Global amplitude

	// Init BackgroundLayer
	mBackground.setup(getWindowWidth(), getWindowHeight(), mOffsetCameratH);

	// Init Wave Model
	mWave.setup(getWindowWidth(), getWindowHeight(), numRows, numLines, -mOffsetCameratH);

	// set a random offset
	Rand rnd;
	rnd.seed((unsigned long)GetTickCount());
	mOffsetTime = rnd.nextFloat(0.0f, 100.0f);

	// Set the Shader program
	mpWaveShader = &mFresnelShader;
	mpWaveShader->load();
}

void CristalWaveApp::update()
{
	float elapsedTime = (float)getElapsedSeconds() * 0.1f + mOffsetTime;
	mOpacity += (mOpacity < 1.0f) ? 0.005f : 0.0f;

	// --------------------------------------------------------
	// Update Background
	Color topColor;
	mBackground.update(elapsedTime * 0.2f, topColor);
	
	// Change color 
	mpWaveShader->setAmbiantColor(topColor);
	mpWaveShader->setDiffuseColor(topColor);

	// --------------------------------------------------------
	// Update Wave
	float speedFactor = (10 - (float)getElapsedSeconds()) * 0.1f;
	if (speedFactor < 0){
		speedFactor = 0.0f;
	}
	// Smooth factor
	speedFactor = speedFactor * speedFactor * (3 - 2 * speedFactor);
	mWave.update(elapsedTime, speedFactor * 0.25f);
	//mWave.update(0);

	// --------------------------------------------------------
	// Change light position
	mpWaveShader->setLightPosition(Vec3f(0.0f, 10.0f, -7.0f + sin(elapsedTime) * 2));
}

void CristalWaveApp::draw()
{
	// --------------------------------------------------------
	// Set Camera for background
	setCameraOrtho(Vec3f(0, 0, 500.0f));
	gl::setMatrices(mCamera);

	// Draw Background
	mBackground.draw();

	// --------------------------------------------------------
	// Set Camera for wave
	setCameraOrtho(Vec3f(0, 140, 1000.0f));
	//setCameraOrtho(Vec3f(1500, 200, 500));
	gl::setMatrices(mCamera);

	// Bind Wave Shader
	mpWaveShader->bind();
	mpWaveShader->pushUniform();

	// --------------------------------------------------------
	// Draw Wave
	mWave.draw();

	//gl::enableAlphaBlending();
	//gl::drawSphere(Vec3f(0.0f, 0.0f, 0.0f), 200.0f, 64);


	// --------------------------------------------------------
	// UnBind Wave Shader
	mpWaveShader->unbind();

#ifdef SHOW_WAVE_TRACE
	// --------------------------------------------------------
	// Trace
	setCameraOrtho(Vec3f(0, 140, 1000.0f), true);
	gl::setMatrices(mCamera);
	mWave.writeInfo(Vec2f(-getWindowWidth() * 0.5f + 50, getWindowHeight() * 0.5f - 270));
#endif	

	// --------------------------------------------------------
	// Fade in effect
	fadeLayer(1.0f - mOpacity);
}

void CristalWaveApp::fadeLayer(float opacity){

	if (opacity > 0.0f)
	{
		// Set Camera for fadeEffect
		setCameraOrtho(Vec3f(0, 0, 500.0f));
		gl::setMatrices(mCamera);

		// Draw mask Layer
		gl::enableAlphaBlending();
		glDisableClientState(GL_COLOR_ARRAY);
		gl::color(ColorA(0.0f, 0.0f, 0.0f, opacity));
		float width = (float)getWindowWidth(),
			height = (float)getWindowHeight(),
			halfWidth = width * 0.5f,
			halfHeight = height * 0.5f;

		gl::drawSolidRect(Rectf(-halfWidth, -halfHeight, halfWidth, halfHeight));
		gl::disableAlphaBlending();
	}
}

// Set Camera
void CristalWaveApp::setCameraOrtho(Vec3f eye, bool reverse){
	Vec3f center = Vec3f(0.0f, 0.0f, 0.0f);
	float sizeW = getWindowWidth() * 0.5f;
	float sizeH = getWindowHeight() * 0.5f;
	if (reverse)
	{
		sizeH *= -1.0f;
	}

	CameraOrtho camera(-sizeW, sizeW, -sizeH, sizeH, 1.0f, 5000.0f);

	camera.setEyePoint(eye);
	camera.setCenterOfInterestPoint(center);
	mCamera = camera;
	mCenter = center;
}


//CINDER_APP_SCREENSAVER(CristalWaveApp, RendererGl(RendererGl::AA_NONE))
#ifdef SCREENSAVER_MODE
CINDER_APP_SCREENSAVER(CristalWaveApp, RendererGl(RendererGl::AA_MSAA_8))
#else
CINDER_APP_BASIC(CristalWaveApp, RendererGl(RendererGl::AA_MSAA_8))
#endif
