#include "cinder/app/AppNative.h"
#include "cinder/Rand.h"
#include "ParticuleManager.h"


ParticuleManager::ParticuleManager(){
	repulsionActivated = false;
}

ParticuleManager::~ParticuleManager(){
	for (vector<Particule*>::iterator it = _particuleList.begin(); it != _particuleList.end(); it++){
		delete *it;
	}

	_particuleList.clear();
}

Vec3f ParticuleManager::computeAttractorForce(Vec3f position){
	return (attrPosition - position) * attrFactor;
}

Vec3f ParticuleManager::computeRepulsionForce(Vec3f position){
	Vec3f repulsionForce;
	float length = 0.0f;

	repulsionForce = (attrPosition - position);
	length = repulsionForce.length();

	repulsionForce = (repulsionForce.length() > 0 && repulsionRadius - repulsionForce.length() >= 0) ? repulsionForce.normalized() : Vec3f::zero();
	return repulsionForce * repulsionFactor;
}

void ParticuleManager::init(int nbParticule, ParticuleManager::PARTICULE_LIFE lifeParameters, Particule::BOX spawnBox){

	life = lifeParameters;

	for (int i = 0; i < nbParticule; i++){
		
		//float radius = randFloat(0.5f, randFloat(0.8f, randFloat(1.2f, randFloat(1.6f, 20.0f))));
		float radius = randFloat(0.5f, randFloat(0.8f, randFloat(1.2f, randFloat(1.6f, randFloat(5.0f, 12.0f)))));
		float mass = randFloat(30.0f, 100.0f);
		float drag = 1.0f;
		Particule* particule = new Particule(spawnBox, radius, mass, drag);
		addParticule(particule);
	}
}

void ParticuleManager::computeParticuleLife(Particule * particule, double elapsedSeconds){
	float ttl = 0.0f, tth = 0.0f;
	float opacity = 0.0f;
	float elapsedSecondsFloat = static_cast<float>(elapsedSeconds);

	if (particule->getTimeToHide() > elapsedSecondsFloat){
		if (particule->getTimeToLive() > elapsedSecondsFloat){

			particule->status = Particule::STATE::LIVE;
			//particule->setColor(Color(0.0f, 1.0f, 0.0f));
			//particule->setOpacity(particule->getOpacity() + elapsedSeconds * 0.025f);
			//particule->setOpacity(sin((elapsedSeconds + particule->getTimeOffset()) * 20.0f) * 0.5f + 0.5f);
			float sfloat1 = static_cast<float>(sin(elapsedSecondsFloat * 0.25f + particule->getTimeOffset()));
			opacity = (static_cast<float>(sin((elapsedSecondsFloat + particule->getTimeOffset()) * 8.0f + sfloat1)) * 0.5f + 0.5f) * 0.75f;
			opacity = opacity * opacity * (3 - 2 * opacity) * 0.75f;
			particule->setOpacity(opacity);
		}
		else{
			particule->status = Particule::STATE::HIDDEN;
			//particule->setColor(Color(1.0f, 0.0f, 0.0f));
			particule->setOpacity(particule->getOpacity() - elapsedSecondsFloat * 0.025f);
		}
		//(*it)->smoothBlink();
	}
	else{
		//particule->setColor(Color(0.0f, 0.0f, 1.0f));
		ttl = (Particule::STATE::NONE != particule->status) ? static_cast<float>(app::getElapsedSeconds()) + randFloat(life.minTTL, life.maxTTL) : 0.0f;
		//ttl = app::getElapsedSeconds() + randFloat(life.minTTL, life.maxTTL);
		tth = ttl + randFloat(life.minTTH, life.maxTTH);

		particule->status = Particule::STATE::HIDDEN;
		particule->setTimeToLive(ttl);
		particule->setTimeToHide(tth);
		particule->newRandomPosition(spawnBox);
	}
}

void ParticuleManager::update(){
	Vec3f force;
	double elapsedSeconds = app::getElapsedSeconds();
	

	for (vector<Particule*>::iterator it = _particuleList.begin(); it != _particuleList.end(); it++){
		
		computeParticuleLife(*it, elapsedSeconds);

		if (repulsionActivated)
		{
			force = computeRepulsionForce((*it)->getPosition());
		}
		else{
			force = computeAttractorForce((*it)->getPosition());
		}

		(*it)->addForces(force);
		(*it)->update();
	}
}

void ParticuleManager::draw(){
	gl::enableAdditiveBlending();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TRIANGLE_FAN);

	for (vector<Particule*>::iterator it = _particuleList.begin(); it != _particuleList.end(); it++){
		(*it)->draw();
	}
	gl::enableAlphaBlending(false);
}

void ParticuleManager::addParticule(Particule* particule){
	_particuleList.push_back(particule);
}

void ParticuleManager::destroyParticule(Particule* particule){
	vector<Particule*>::iterator it = find(_particuleList.begin(), _particuleList.end(), particule);
	if (*it != nullptr)
	{
		delete *it;
	}
	_particuleList.erase(it);
}

void ParticuleManager::setRepulsion(bool activated, Vec3f position){
	repulsionActivated = activated;
	repulsionPosition = position;
}

