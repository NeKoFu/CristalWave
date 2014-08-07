#pragma once

#include <vector>
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/perlin.h"
#include "SparkleParticule.h"
#include "ParticuleManager.h"

using namespace std;

class ParticuleInTheWindManager : public ParticuleManager
{
	float _frequency;
	Perlin _perlin;
	float _screenWidth;
	Timer _timer;
	gl::Texture _texture;

public:
	ParticuleInTheWindManager() 
		:ParticuleManager(){
		_frequency = 0.01f;
		_perlin = Perlin();
	};

	void update();
	void draw();
	void init(int nbParticule, PARTICULE_LIFE lifeParameters, float screenWidth);

protected:
	void computeParticuleLife(Particule * particule, double elapsedSeconds);
};