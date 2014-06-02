#pragma once

#include <vector>
#include "cinder/gl/gl.h"
#include "cinder/perlin.h"
#include "Particule.h"
#include "ParticuleManager.h"

using namespace std;

class ParticuleInTheWindManager : public ParticuleManager
{
	float _frequency;
	Perlin _perlin;

public:
	ParticuleInTheWindManager() 
		:ParticuleManager(){
		_frequency = 0.01f;
		_perlin = Perlin();
	};

	void update();
};