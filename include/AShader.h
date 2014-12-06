#pragma once
// ---------------------------------------------------------
//
//
//
// ---------------------------------------------------------

#include "cinder/gl/gl.h" 
#include "cinder/gl/GlslProg.h"
#include "cinder/app/AppScreenSaver.h"
#include "Resources.h"

using namespace ci;

class AShader{
protected:
	char * mShaderName;
	short mIdRessourceVertexShader, mIdRessourceFragmentShader;
	gl::GlslProg mShaderProgram;
	Color mColorAmbient;
	Color mColorDiffuse;
	Color mColorSpecular;
	Vec3f mLightPosition;
	float mOpacity;

public:
	virtual void load();
	virtual void pushUniform() = 0;
	virtual void bind();
	virtual void unbind();

	virtual void setAmbiantColor(Color c);
	virtual Color getAmbiantColor();

	virtual void setDiffuseColor(Color c);
	virtual Color getDiffuseColor();

	virtual void setSpecularColor(Color c);
	virtual Color getSpecularColor();

	virtual void setOpacity(float opacity);
	virtual float getOpacity();

	virtual void setLightPosition(Vec3f position);
	virtual Vec3f getLightPosition();
};