#pragma once

#include "AShader.h"

class SparkleShader : public AShader{
public:
	SparkleShader(float level = 1.0f, Color color = Color(1.0f, 1.0f, 1.0f));
	void load();
	void pushUniform();
	inline void setAdditiveFactor(float factor) {
		mAdditiveFactor = factor;
	}

	inline void setSpriteId(int sprite){
		mSpriteId = sprite;
	}

private:
	float mAdditiveFactor;
	int mSpriteId;
	Color mColor;
};