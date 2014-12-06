#pragma once

#include <vector>
#include "cinder/gl/gl.h"
#include "Particule.h"

using namespace std;

class ParticuleManager
{
public:
	struct PARTICULE_LIFE{
	public:
		float minTTL, maxTTL, minTTH, maxTTH;

		PARTICULE_LIFE(float minTTLValue = 0.0f, float maxTTLValue = 0.0f, float minTTHValue = 0.0f, float maxTTHValue = 0.0f){
			minTTL = minTTLValue;
			maxTTL = maxTTLValue;
			minTTH = minTTHValue;
			maxTTH = maxTTHValue;
		};
	};

protected:
	vector<Particule*> _particuleList;
	bool repulsionActivated;

	Vec3f computeAttractorForce(Vec3f position);
	Vec3f computeRepulsionForce(Vec3f position);
	virtual void computeParticuleLife(Particule & particule, float elapsedSeconds);

public:
	Vec3f attrPosition
		, repulsionPosition;
	float attrFactor
		, repulsionFactor
		, repulsionRadius;
	
	PARTICULE_LIFE life;
	Particule::ISPRAY & spray;

	ParticuleManager(Particule::ISPRAY & emitter);
	~ParticuleManager();

	virtual void init(const int nbParticule, PARTICULE_LIFE lifeParameters);
	virtual void update();
	virtual void draw();

	void addParticule(Particule* particule);
	void destroyParticule(Particule* particule);

	void setRepulsion(bool activated, Vec3f position = Vec3f::zero());
};