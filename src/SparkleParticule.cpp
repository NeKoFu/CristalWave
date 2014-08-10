#include "SparkleParticule.h"

SparkleParticule::SparkleParticule(const Particule::BOX playBox, float radius, float mass, float drag, SparkleShader &shader)
	: Particule(playBox, radius, mass, drag), _shader(shader){

	//_shader = shader;

	float limiter = 1.0f;

	_nbAdditiveFactor = randFloat(1, 16);
	
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

void SparkleParticule::draw(){

	float radius = _radius * _scale;
	if (_opacity > 0.0f)
	{
		_shader.setAdditiveFactor(_nbAdditiveFactor);
		_shader.setAmbiantColor(_color);
		_shader.setOpacity(_opacity);
		_shader.setSpriteId(_sprite);
		_shader.pushUniform();

		// Todo : replace by point sprite
		gl::drawSolidRect(Rectf(_currentPosition.x - radius, _currentPosition.y - radius
			, _currentPosition.x + radius, _currentPosition.y + radius));
	}
}