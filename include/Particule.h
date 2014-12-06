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
	struct EMITTER{
		enum EMITTER_TYPE{
			NONE
			, SPHERE
			, BOX
		};
	};

	struct STATE{
		enum STATE_TYPE{
			NONE
			, LIVE
			, HIDDEN
		};
	};

	STATE::STATE_TYPE status;

	

	struct ISPRAY{
	public:
		Vec3f position;
		EMITTER::EMITTER_TYPE type = EMITTER::NONE;
		virtual bool isInside(Vec3f p) const = 0;
		virtual Vec3f randomPoint() const = 0;
	};

	struct SPHERE : ISPRAY{
	public:
		float radius;
		const EMITTER::EMITTER_TYPE type = EMITTER::SPHERE;

		SPHERE(Vec3f p = { 0.0f, 0.0f, 0.0f }, float r = 1.0f) {
			position = p;
			radius = r;
		};

		inline bool isInside(Vec3f p) const{
			return (position.dot(p) < radius);
		}

		inline Vec3f randomPoint() const{
			float radiusSquare = radius * radius;
			float x = randFloat(-radius, radius);
			float yLimit = sqrt(radiusSquare - x * x);
			float y = randFloat(-yLimit, yLimit);
			float zLimit = sqrt(yLimit * yLimit - y * y);
			float z = randFloat(-zLimit, zLimit);
			return Vec3f(x + position.x, y + position.y, z + position.z);
		};
	};

	struct BOX : ISPRAY{
	public:
		float x1, x2, y1, y2, z1, z2;
		const EMITTER::EMITTER_TYPE type = EMITTER::BOX;
		
		BOX(Vec3f p = { 0.0f, 0.0f, 0.0f }, float width = 1.0f, float height = 1.0f, float depth = 1.0f) {
			position = p;
			x1 = p.x - width * 0.5f;
			x2 = p.x + width * 0.5f;
			y1 = p.y - height * 0.5f;
			y2 = p.y - height * 0.5f;
			z1 = p.z - depth * 0.5f;
			z2 = p.z - depth * 0.5f;
		};

		BOX(float xLeft, float xRight, float yLeft, float yRight, float zLeft, float zRight) {
			x1 = xLeft;
			x2 = xRight;
			y1 = yLeft;
			y2 = yRight;
			z1 = zLeft;
			z2 = zRight;
			position = Vec3f((x1 + x2) * 0.5f
						, (y1 + y2) * 0.5f
						, (z1 + z2) * 0.5f);
		};

		inline bool isInside(Vec3f p) const{
			return (x1 < p.x && x2 > p.x
				&&  y1 < p.y && y2 > p.y
				&&  z1 < p.z && z2 > p.z);
		}

		inline Vec3f randomPoint() const{
			float x = randFloat(x1, x2);
			float y = randFloat(y1, y2);
			float z = randFloat(z1, z2);
			return Vec3f(x + position.x, y + position.y, z + position.z);
		};
	};

protected:
	Vec3f _currentPosition
		, _previousPosition
		, _forces;

	ISPRAY & _spray;
	
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

	Particule(ISPRAY & spray, float radius, float mass, float drag);

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

	//virtual inline void setEmitter(const ISPRAY & spray){
	//	_spray = spray;
	//}

	virtual inline const ISPRAY & getEmitter(){
		return _spray;
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

	virtual inline void newRandomPosition(){
		_currentPosition = _spray.randomPoint();
		_previousPosition = _currentPosition;
	};

protected:
	virtual void setMaterial();
};