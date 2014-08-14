#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace std;

class Particule
{
public:
	enum STATE{
		NONE
		, LIVE
		, HIDDEN
	};

	STATE status;

	struct BOX{
	public:
		float x1, x2, y1, y2, z1, z2;
		
		BOX(float xLeft = 0.0f, float xRight = 0.0f, float yLeft = 0.0f, float yRight = 0.0f, float zLeft = 0.0f, float zRight = 0.0f) {
			x1 = xLeft;
			x2 = xRight;
			y1 = yLeft;
			y2 = yRight;
			z1 = zLeft;
			z2 = zRight;
		};
	};

protected:
	Vec3f _currentPosition
		, _previousPosition
		, _forces;

	BOX _playBox;
	
	float _radius
		, _scale
		, _mass
		, _drag
		, _ttl
		, _tth
		, _ttoffset;

	Color _color;
	float _opacity;

public:

	Particule(const BOX playBox, float radius, float mass, float drag);

	void update();
	void draw();

	virtual inline float getRadius(){
		return _radius;
	}

	virtual inline void setRadius(float radius) {
		_radius = radius;
	}

	virtual inline float getScale(){
		return _scale;
	}

	virtual inline void setScale(float scale) {
		_scale = scale;
	}

	virtual inline void setEmitterBox(BOX box){
		_playBox = box;
	}

	virtual inline Vec3f getPosition(){
		return _currentPosition;
	}

	virtual inline void setPosition(Vec3f position){
		_currentPosition = position;
	}

	virtual inline float getTimeToLive(){
		return _ttl;
	}

	virtual inline void setTimeToLive(float value){
		_ttl = value;
	}

	virtual inline float getTimeToHide(){
		return _tth;
	}

	virtual inline void setTimeToHide(float value){
		_tth = value;
	}

	virtual inline void addForces(Vec3f forces){
		_forces += forces;
	}

	virtual inline float getOpacity(){
		return _opacity;
	}

	virtual inline void setOpacity(float opacity){
		opacity = (opacity < 1.0f) ? opacity : 1.0f;
		_opacity = (opacity > 0.0f) ? opacity : 0.0f;
	}

	virtual inline Color getColor(){
		return _color;
	}

	virtual inline void setColor(Color color){
		_color = color;
	}

	virtual inline float getTimeOffset(){
		return _ttoffset;
	}

	virtual inline void newRandomPosition(BOX box){
		_playBox = box;
		newRandomPosition();
	}

	virtual inline void newRandomPosition(){
		float x = randFloat(_playBox.x1, _playBox.x2);
		float y = randFloat(_playBox.y1, _playBox.y2);
		float z = randFloat(_playBox.z1, _playBox.z2);
		_currentPosition = Vec3f(x, y, z);
		_previousPosition = _currentPosition;
	};

protected:
	virtual void setMaterial();
};