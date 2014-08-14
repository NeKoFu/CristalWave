#include "Particule.h"

Particule::Particule(const Particule::BOX playBox, float radius, float mass, float drag){
	_playBox = playBox;
	_radius = radius;
	_scale = 1.0f;
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
	float radius = _radius * _scale;
	if (_opacity > 0.0f)
	{
		setMaterial();

		//gl::drawSolidCircle(Vec2f(_currentPosition.x, _currentPosition.y), _radius);
		gl::drawSolidRect(Rectf(_currentPosition.x - radius, _currentPosition.y - radius
								, _currentPosition.x + radius, _currentPosition.y + radius));
	}
}

void Particule::setMaterial(){
	gl::color(_color.r, _color.g, _color.b, _opacity);
}