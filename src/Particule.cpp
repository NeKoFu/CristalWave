#include "Particule.h"

Particule::Particule(const Particule::BOX playBox, float radius, float mass, float drag){
	_playBox = playBox;
	_radius = radius;
	_mass = (mass > 0.00001f || mass < -0.00001f) ? mass : 1.0f;
	_drag = drag;
	_ttl = 0.0f;
	_tth = 0.0f;
	_forces = Vec3f::zero();
	_opacity = 0.0f;
	_color = Color(1.0f, 1.0f, 1.0f);
	_ttoffset = randFloat(0.0f, 10.0f);
	status = Particule::STATE::NONE;
	newRandomPosition();
}

void Particule::update(){
	Vec3f velocity = (_currentPosition - _previousPosition) * _drag;
	_previousPosition = _currentPosition;
	_currentPosition += velocity + _forces / _mass;
	_forces = Vec3f::zero();
}

void Particule::draw(){
	//gl::drawCube(_currentPosition, Vec3f(_radius, _radius, _radius));
	//gl::color(_color.r, _color.g, _color.b, 1.0f);
	if (_opacity > 0.0f)
	{
		gl::color(_color.r, _color.g, _color.b, _opacity);
		//gl::drawSphere(_currentPosition, _radius);
		gl::drawSolidCircle(Vec2f(_currentPosition.x, _currentPosition.y), _radius);
	}
	else
	{
		//gl::color(_color.r, 0.0f, 0.0f, 1.0f);
		//gl::drawSphere(_currentPosition, _radius);
	}
	
}