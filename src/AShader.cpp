// ---------------------------------------------------------
//
//
//
// ---------------------------------------------------------

#include "AShader.h"

using namespace ci::app;

void AShader::load(){
	try
	{
		mShaderProgram = gl::GlslProg(loadResource(mIdRessourceVertexShader, "GLSL"), loadResource(mIdRessourceFragmentShader, "GLSL"));
	}
	catch (gl::GlslProgCompileExc cex)
	{
		console() << "Unable to compile the shaders programs [ " << mShaderName << " ]." << std::endl;
		console() << "Shader Log: " << std::endl << cex.what() << std::endl;
		throw(new Exception());
	}
	catch (...)
	{
		console() << "Unable to load the shaders programs [ " << mShaderName << " ]." << std::endl;
		console() << "Shader Log: " << std::endl << mShaderProgram.getShaderLog(0) << std::endl;
		throw(new Exception());
	}
}

void AShader::bind(){
	mShaderProgram.bind();
}

void AShader::unbind(){
	mShaderProgram.unbind();
}

void AShader::setAmbiantColor(Color c){
	mColorAmbient = c;
}

Color AShader::getAmbiantColor(){
	return mColorAmbient;
}

void AShader::setDiffuseColor(Color c){
	mColorDiffuse = c;
}

Color AShader::getDiffuseColor(){
	return mColorDiffuse;
}

void AShader::setSpecularColor(Color c){
	mColorSpecular = c;
}

Color AShader::getSpecularColor(){
	return mColorSpecular;
}

void AShader::setOpacity(float opacity){
	mOpacity = opacity;
}

float AShader::getOpacity(){
	return mOpacity;
}

void AShader::setLightPosition(Vec3f position){
	mLightPosition = position;
}

Vec3f AShader::getLightPosition(){
	return mLightPosition;
}

