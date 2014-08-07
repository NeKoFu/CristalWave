#include "SparkleParticule.h"

SparkleParticule::SparkleParticule(const Particule::BOX playBox, float radius, float mass, float drag)
	: Particule(playBox, radius, mass, drag){

	float limiter = 1.0f;

	_nbRepeatDraw = randInt(1, 5);
	
	if (radius > 3) {
		limiter *= 3 / radius;
	}

	if (_nbRepeatDraw > 4) {
		_nbRepeatDraw *= 3 * limiter;
	}else if (_nbRepeatDraw > 2) {
		_nbRepeatDraw *= 2 * limiter;
	}

	
}

void SparkleParticule::draw(){
	for (unsigned short i = 0; i < _nbRepeatDraw; i++){
		Particule::draw();
	}
}