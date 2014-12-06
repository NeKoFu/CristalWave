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
	int _screenWidth;
	Timer _timer;
	gl::Texture _texture;
	SparkleShader _shader;
	SparkleShader::Attributes _shaderAttributes;

public:
	ParticuleInTheWindManager(Particule::ISPRAY & emitter)
		:ParticuleManager(emitter){
		_frequency = 0.01f;
		_perlin = Perlin();
	};

	void update();
	void draw();
	void drawBatch();
	void init(const int nbParticule, PARTICULE_LIFE lifeParameters, int screenWidth);
	void setColor(Color color);

protected:
	void computeParticuleLife(SparkleParticule & particule, float elapsedSeconds);
};