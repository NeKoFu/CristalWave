#include "cinder/app/AppNative.h"
#include "ParticuleInTheWindManager.h"

void ParticuleInTheWindManager::update(){
	float elapsedSeconds = app::getElapsedSeconds();
	//Vec3f oscilation = Vec3f(sin(elapsedSeconds * 0.5f) * 0.2f, sin(elapsedSeconds * 0.2f) * 0.1f, 0.0f);
	//Vec3f oscilation = Vec3f(sin(elapsedSeconds * 0.6f) * 0.2f, sin(elapsedSeconds * 0.2f) * 0.1f, 0.0f);
	//Vec3f oscilation = Vec3f(sin(elapsedSeconds * 0.3f) * 0.2f, sin(elapsedSeconds * 0.1f) * 0.1f, 0.0f);
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
