#include "cinder/app/AppNative.h"
#include "ParticuleInTheWindManager.h"
#include "Resources.h"

using namespace ci::app;

void ParticuleInTheWindManager::update(){
	float elapsedSeconds = app::getElapsedSeconds();
	Vec3f oscilation = Vec3f(sin(elapsedSeconds * 0.3f) * 0.88f, sin(elapsedSeconds * 0.3f) * 0.24f, 0.0f);
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
			particule->setOpacity(particule->getOpacity() - elapsedSeconds * 0.025f);
		}
	}
	// spawn to a new position only if framerate is fast enough
	else if (static_cast<float>(_timer.getSeconds()) < 0.5f) {
		float leftLimit = -_screenWidth * 0.3f;
		float rightLimit = _screenWidth * 0.3f;
		if (particule->getRadius() < 3.5f
			|| spawnBox.x1 < leftLimit
			|| spawnBox.x2 > rightLimit){
			
			ttl = (Particule::STATE::NONE != particule->status) ? app::getElapsedSeconds() + randFloat(life.minTTL, life.maxTTL) : 0.0f;
			tth = ttl + randFloat(life.minTTH, life.maxTTH);

			particule->status = Particule::STATE::HIDDEN;
			particule->setTimeToLive(ttl);
			particule->setTimeToHide(tth);
			particule->newRandomPosition(spawnBox);
		}
	}
	_timer.start();
}

void ParticuleInTheWindManager::init(int nbParticule, PARTICULE_LIFE lifeParameters, float screenWidth){
	_screenWidth = screenWidth;
	_timer.start();

	_texture = gl::Texture(loadImage(loadResource(RES_TEXTURE_PARTICLE_GLOW, "IMAGE")));

	Particule::BOX spawnBox = Particule::BOX(0, 0, 0, 0, 0, 0);

	life = lifeParameters;
	for (int i = 0; i < nbParticule; i++){

		float radius = randFloat(0.5f, randFloat(0.8f, randFloat(1.2f, randFloat(1.6f, randFloat(5.0f, 18.0f)))));
		float mass = randFloat(30.0f, 100.0f);
		float drag = 1.0f;
		SparkleParticule* particule = new SparkleParticule(spawnBox, radius, mass, drag);
		addParticule(particule);
	}
}

void ParticuleInTheWindManager::draw(){
	_texture.enableAndBind();
	//ParticuleManager::draw();

	gl::enableAdditiveBlending();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TRIANGLE_FAN);

	for (vector<Particule*>::iterator it = _particuleList.begin(); it != _particuleList.end(); it++){
		(static_cast<SparkleParticule*>(*it))->draw();
	}
	gl::enableAlphaBlending(false);

	_texture.disable();

}