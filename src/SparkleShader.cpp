
#include "SparkleShader.h"

SparkleShader::SparkleShader(float additiveFactor /* = 1.0f*/, Color color /* Color(1.0f, 1.0f, 1.0f) */){
	mShaderName = "Sparkle";
	mIdRessourceVertexShader = RES_VERT_GLSL_PASSTHRU;
	mIdRessourceFragmentShader = RES_FRAG_GLSL_SPARKLE;

	setAdditiveFactor(additiveFactor);
	mColorAmbient = color;
}

void SparkleShader::load() {
	AShader::load();
};

void SparkleShader::pushUniform()
{
	mShaderProgram.uniform("uAdditiveFactor", mAdditiveFactor);
	mShaderProgram.uniform("uSpriteId", mSpriteId);
	mShaderProgram.uniform("uColor", Vec4f(mColorAmbient.r, mColorAmbient.g, mColorAmbient.b, mOpacity));
}
