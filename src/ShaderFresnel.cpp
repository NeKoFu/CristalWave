
#include "ShaderFresnel.h"

ShaderFresnel::ShaderFresnel(){
	mShaderName = "Fresnel";
	mIdRessourceVertexShader = RES_VERT_GLSL_FRESNEL;
	mIdRessourceFragmentShader = RES_FRAG_GLSL_FRESNEL;

	mDiffuseK = 1.33f;
	mRoughness = 0.0010f;
	mIndexOfRefraction = 1.31f;
	mColorAmbient = Color(0.1f, 0.1f, 0.2f);
	mColorDiffuse = Color(0.0f, 0.1f, 0.4f);
	mColorSpecular = Color(1.0f, 1.0f, 1.0f);
	mSpecularAttenuation = 8;
	mTransluscence = 0.05f;
	mLightPosition = Vec3f(0.0f, 10.0f, -7.0f);
}

void ShaderFresnel::load() {
	AShader::load();
};

void ShaderFresnel::pushUniform()
{
	mShaderProgram.uniform("uK", mDiffuseK);
	mShaderProgram.uniform("uRoughness", mRoughness);
	mShaderProgram.uniform("uIoR", mIndexOfRefraction);
	mShaderProgram.uniform("uAmbient", Vec3f(mColorAmbient.r, mColorAmbient.g, mColorAmbient.b));
	mShaderProgram.uniform("uDiffuse", Vec3f(mColorDiffuse.r, mColorDiffuse.g, mColorDiffuse.b));
	mShaderProgram.uniform("uSpecular", Vec3f(mColorSpecular.r, mColorSpecular.g, mColorSpecular.b));
	mShaderProgram.uniform("uSpecularAttenuation", mSpecularAttenuation);
	mShaderProgram.uniform("uTransluscence", mTransluscence);
	mShaderProgram.uniform("uLightPosition", mLightPosition);
}
