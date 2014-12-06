#include "cinder/app/AppNative.h"
#include "ParticuleInTheWindManager.h"
#include "Resources.h"

using namespace ci::app;

void ParticuleInTheWindManager::setColor(Color color){
	_shader.setAmbiantColor(color + 0.05f);
}

void ParticuleInTheWindManager::init(const int nbParticule, PARTICULE_LIFE lifeParameters, int screenWidth){
	_screenWidth = screenWidth;
	_timer.start();

	_shader.load();

	life = lifeParameters;
	for (int i = 0; i < nbParticule; i++){

		float radius = randFloat(1.0f, randFloat(1.2f, randFloat(1.6f, randFloat(2.0f, randFloat(4.0f, 64.0f)))));
		float mass = randFloat(80.0f, 250.0f);
		float drag = 1.0f;
		SparkleParticule* particule = new SparkleParticule(spray, radius, mass, drag, _shader);
		addParticule(particule);
	}

	// point sprite parameters
	_shaderAttributes.positions = new Vec3f[nbParticule];
	_shaderAttributes.radius = new float[nbParticule]; // size
	_shaderAttributes.colors = new Vec4f[nbParticule]; // color, opacity
	_shaderAttributes.spriteIDs = new int[nbParticule]; // spriteID
	_shaderAttributes.additiveFactors = new float[nbParticule]; // additiveFactor
	_shaderAttributes.texture = gl::Texture(loadImage(loadResource(RES_TEXTURE_PARTICLE_GLOW, "IMAGE")));
}

void ParticuleInTheWindManager::update(){
	float elapsedSeconds = static_cast<float>(app::getElapsedSeconds());
	Vec3f oscilation = Vec3f(sin(elapsedSeconds * 0.3f) * 0.88f, sin(elapsedSeconds * 0.3f) * 0.24f, 0.0f);
	Vec3f attrForce;
	Vec3f force;
	Color spriteColor = _shader.getAmbiantColor();

	// batch each particles
	int i = 0;
	for (vector<Particule*>::iterator it = _particuleList.begin(); it != _particuleList.end(); it++){
		
		// Set visible life
		computeParticuleLife((*(SparkleParticule*)*it), elapsedSeconds);

		// Attract to center
		attrPosition.x = (*it)->getPosition().x;
		attrForce = (attrPosition - (*it)->getPosition()) * attrFactor  * oscilation * 5;
		attrForce *= attrForce; // Effervescent style
		(*it)->addForces(attrForce);

		// Add noise 
		force = _perlin.dfBm((*it)->getPosition() * _frequency) * 2.2f;
		(*it)->addForces(force);
		(*it)->addForces(oscilation);
		(*it)->update();


		// Point sprites parameters
		_shaderAttributes.positions[i] = (*it)->getPosition();
		_shaderAttributes.colors[i].x = ((*it)->status != Particule::STATE::HIDDEN) ? spriteColor.r : 0.0; // color
		_shaderAttributes.colors[i].y = ((*it)->status != Particule::STATE::HIDDEN) ? spriteColor.g : 0.0; // color
		_shaderAttributes.colors[i].z = ((*it)->status != Particule::STATE::HIDDEN) ? spriteColor.b : 0.0; // color
		_shaderAttributes.colors[i].w = (*it)->getOpacity() * 2.0f; // opacity
		_shaderAttributes.spriteIDs[i] = ((SparkleParticule*)*it)->getSprite(); // spriteID
		_shaderAttributes.radius[i] = (*it)->getRadius() * (*it)->getScale(); // size
		_shaderAttributes.additiveFactors[i] = ((SparkleParticule*)*it)->getAdditiveFactor(); // additiveFactor
		i++;
	}
}


void ParticuleInTheWindManager::computeParticuleLife(SparkleParticule & particule, float elapsedSeconds){
	float ttl = 0.0f, tth = 0.0f;
	float opacity = 0.0f;

	if (particule.getTimeToHide() > elapsedSeconds){
		if (particule.getTimeToLive() > elapsedSeconds){

			particule.status = Particule::STATE::LIVE;

			opacity = (
				static_cast<float>(sin((elapsedSeconds + particule.getTimeOffset()) * 8.0f
					+ static_cast<float>(sin(elapsedSeconds * 2.33f + particule.getTimeOffset()))
				))
				* 0.5f + 0.5f) * 0.7f;
			opacity = opacity * opacity * (3 - 2 * opacity);


			// set smooth scale down at start
			float scale = 1.0f;
			if (elapsedSeconds <= 5.0){
				scale = (5.0f - elapsedSeconds) * 0.2f;
				if (scale <= 0.0f)
				{
					scale = 1.0f;
				}
				else {
					scale = scale * scale * (3 - 2 * scale);
					opacity -= scale;
					scale = scale * 24 + 1.0f;
				}
			}

			particule.setScale(scale);
			particule.setOpacity(opacity);

		}
		else{
			particule.status = Particule::STATE::HIDDEN;
			particule.setOpacity(particule.getOpacity() - elapsedSeconds * 0.025f);
		}
	}
	// spawn to a new position only if framerate is fast enough
	else if (static_cast<float>(_timer.getSeconds()) < 0.5f) {
		float limit = 5 / particule.getRadius();
		limit *= limit * limit;
		float leftLimit = (-_screenWidth * 0.45f) + _screenWidth * limit;
		float rightLimit = (_screenWidth * 0.45f) - _screenWidth * limit;

		if (particule.getRadius() < 3.0f
			|| spray.position.x < leftLimit
			|| spray.position.x > rightLimit){
			
			ttl = (Particule::STATE::NONE != particule.status) ? static_cast<float>(app::getElapsedSeconds()) + randFloat(life.minTTL, life.maxTTL) : 0.0f;
			tth = ttl + randFloat(life.minTTH, life.maxTTH);

			particule.status = Particule::STATE::HIDDEN;
			particule.setTimeToLive(ttl);
			particule.setTimeToHide(tth);
			particule.newRandomPosition();
		}
	}
	_timer.start();
}

void ParticuleInTheWindManager::draw(){
	_shaderAttributes.texture.enableAndBind();
	_shader.bind();

	gl::enableAdditiveBlending();
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	for (vector<Particule*>::iterator it = _particuleList.begin(); it != _particuleList.end(); it++){
		(static_cast<SparkleParticule*>(*it))->draw();
	}

	gl::enableAlphaBlending(false);
	_shader.unbind();
	_shaderAttributes.texture.disable();

}

void ParticuleInTheWindManager::drawBatch(){
	_shader.bind(_shaderAttributes);
	glDrawArrays(GL_POINTS, 0, _particuleList.size());
	_shader.unbind();

}