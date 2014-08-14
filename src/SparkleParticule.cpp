#include "SparkleParticule.h"

SparkleParticule::SparkleParticule(const Particule::BOX playBox, float radius, float mass, float drag, SparkleShader &shader)
	: Particule(playBox, radius, mass, drag), _shader(shader){

	float limiter = 1.0f;

	_nbAdditiveFactor = randFloat(1, 18);
	
	if (radius > 3) {
		limiter *= 3 / radius;
	}

	if (_nbAdditiveFactor > 4) {
		_nbAdditiveFactor *= 3 * limiter;
	}
	else if (_nbAdditiveFactor > 2) {
		_nbAdditiveFactor *= 2 * limiter;
	}

	// choose shape
	_sprite = randInt(1, 10);
	_sprite = (_sprite < 6) ? 0 : 1;
	
}

void SparkleParticule::setMaterial(){
	_shader.setAdditiveFactor(_nbAdditiveFactor);
	_shader.setOpacity(_opacity);
	_shader.setSpriteId(_sprite);
	_shader.pushUniform();
}