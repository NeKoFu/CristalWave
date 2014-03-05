#pragma once

#include "AShader.h"

class ShaderFresnel : public AShader{
public:
	ShaderFresnel();
	void load();
	void pushUniform();

private:
	float mDiffuseK;
	float mRoughness;
	float mIndexOfRefraction;
	float mSpecularAttenuation;
	float mTransluscence;
	float mCounter;
};