#pragma once

#include "Particule.h"
#include "SparkleShader.h"

using namespace ci;
using namespace std;

class SparkleParticule : public Particule
{
	float _nbAdditiveFactor;
	int _sprite;
	SparkleShader &_shader;

public:
	SparkleParticule(const BOX playBox, float radius, float mass, float drag, SparkleShader &shader);

	virtual inline void setOpacity(float opacity){
		opacity /= _scale * 4;

		if (opacity > 1.0f) {
			opacity = 1.0f;
		}

		_opacity = (opacity > 0.0f) ? opacity : 0.0f;
	}

	virtual inline void setScale(float scale) {
		_scale = randFloat(scale, scale * 1.5f);
	}

protected:
	virtual void setMaterial();
};