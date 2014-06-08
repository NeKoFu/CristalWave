#include "cinder/app/AppNative.h"
#include "ParticuleInTheWindManager.h"

void ParticuleInTheWindManager::update(){
	float elapsedSeconds = app::getElapsedSeconds();
	Vec3f oscilation = Vec3f(sin(elapsedSeconds * 0.3f) * 0.2f, sin(elapsedSeconds * 0.3f) * 0.2f, 0.0f);
	Vec3f attrForce;
	Vec3f force;

	for (vector<Particule*>::iterator it = _particuleList.begin(); it != _particuleList.end(); it++){
		
		// Set visible life
		computeParticuleLife(*it, elapsedSeconds);

		// Attract to center
		attrPosition.x = (*it)->getPosition().x;
		attrForce = (attrPosition - (*it)->getPosition()) * attrFactor  * oscilation * 10;
		//attrForce *= attrForce; // Effervescent style
		(*it)->addForces(attrForce);

		// Add noise 
		force = _perlin.dfBm((*it)->getPosition() * _frequency) * 1.8f;
		(*it)->addForces(force);
		(*it)->addForces(oscilation);
		(*it)->update();
	}
}


void ParticuleInTheWindManager::computeParticuleLife(Particule * particule, double elapsedSeconds){
	float ttl = 0.0f, tth = 0.0f;
	float opacity = 0.0f;

	if (particule->getTimeToHide() > elapsedSeconds){
		if (particule->getTimeToLive() > elapsedSeconds){

			particule->status = Particule::STATE::LIVE;
			opacity = (sin((elapsedSeconds + particule->getTimeOffset()) * 8.0f + sin(elapsedSeconds * 2.33f + particule->getTimeOffset())) * 0.5f + 0.5f) * 0.75f;
			opacity = opacity * opacity * (3 - 2 * opacity) * 0.85f;
			particule->setOpacity(opacity);
		}
		else{
			particule->status = Particule::STATE::HIDDEN;
			//particule->setColor(Color(1.0f, 0.0f, 0.0f));
			particule->setOpacity(particule->getOpacity() - elapsedSeconds * 0.025f);
		}
	}
	else {
		float leftLimit = -_screenWidth * 0.35f;
		float rightLimit = _screenWidth * 0.35f;
		if (particule->getRadius() < 3.5f
			|| spawnBox.x1 < leftLimit
			|| spawnBox.x2 > rightLimit){
			
			//particule->setColor(Color(0.0f, 0.0f, 1.0f));
			ttl = (Particule::STATE::NONE != particule->status) ? app::getElapsedSeconds() + randFloat(life.minTTL, life.maxTTL) : 0.0f;
			tth = ttl + randFloat(life.minTTH, life.maxTTH);

			particule->status = Particule::STATE::HIDDEN;
			particule->setTimeToLive(ttl);
			particule->setTimeToHide(tth);
			particule->newRandomPosition(spawnBox);
		}
	}
}

void ParticuleInTheWindManager::init(int nbParticule, PARTICULE_LIFE lifeParameters, float screenWidth){
	_screenWidth = screenWidth;
	Particule::BOX spawnBox = Particule::BOX(0, 0, 0, 0, 0, 0);
	((ParticuleManager*)this)->init(nbParticule, lifeParameters, spawnBox);
}
