#pragma once

#include "cinder/gl/gl.h"
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

private:
	Vec3f _currentPosition
		, _previousPosition
		, _forces;

	BOX _playBox;
	
	float _radius
		, _mass
		, _drag
		, _ttl
		, _tth
		, _ttoffset;

	Color _color;
	float _opacity;

public:

	inline void setEmitterBox(BOX box){
		_playBox = box;
	}

	inline Vec3f getPosition(){
		return _currentPosition;
	}

	inline void setPosition(Vec3f position){
		_currentPosition = position;
	}

	inline float getTimeToLive(){
		return _ttl;
	}

	inline void setTimeToLive(float value){
		_ttl = value;
	}

	inline float getTimeToHide(){
		return _tth;
	}

	inline void setTimeToHide(float value){
		_tth = value;
	}

	inline void addForces(Vec3f forces){
		_forces += forces;
	}

	inline float getOpacity(){
		return _opacity;
	}

	inline void setOpacity(float opacity){
		opacity = (opacity < 1.0f) ? opacity : 1.0f;
		_opacity = (opacity > 0.0f) ? opacity : 0.0f;
	}

	inline Color getColor(){
		return _color;
	}

	inline void setColor(Color color){
		_color = color;
	}

	inline float getTimeOffset(){
		return _ttoffset;
	}

	inline void newRandomPosition(BOX box){
		_playBox = box;
		newRandomPosition();
	}

	inline void newRandomPosition(){
		float x = randFloat(_playBox.x1, _playBox.x2);
		float y = randFloat(_playBox.y1, _playBox.y2);
		float z = randFloat(_playBox.z1, _playBox.z2);
		_currentPosition = Vec3f(x, y, z);
		_previousPosition = _currentPosition;
	};

	

	Particule(const BOX playBox, float radius, float mass, float drag);

	void update();
	void draw();

};