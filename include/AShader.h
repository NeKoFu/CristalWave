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
	void load();
	virtual void pushUniform() = 0;
	void bind();
	void unbind();

	void setAmbiantColor(Color c);
	void setDiffuseColor(Color c);
	void setSpecularColor(Color c);
	void setOpacity(float opacity);
	void setLightPosition(Vec3f position);
};