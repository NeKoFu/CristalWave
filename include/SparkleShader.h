#pragma once

#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "AShader.h"

using namespace std;

class SparkleShader : public AShader{

public:
	struct Attributes {
		Vec3f* positions; // list of sprite positions
		float* radius; // list of sprite sizes
		Vec4f* colors; // color, opacity
		int* spriteIDs; // list of spriteID
		float* additiveFactors; // list of additive factors
		gl::Texture texture; // texture to apply

		Attributes(){
			positions = nullptr;
			radius = nullptr;
			colors = nullptr;
			spriteIDs = nullptr;
			additiveFactors = nullptr;
		}

		~Attributes(){
			if (positions != nullptr){
				delete[] positions;
			}

			if (radius != nullptr){
				delete[] radius;
			}

			if (colors != nullptr){
				delete[] colors;
			}

			if (spriteIDs != nullptr){
				delete[] spriteIDs;
			}

			if (additiveFactors != nullptr){
				delete[] additiveFactors;
			}
		}
	};

public:
	SparkleShader(float level = 1.0f, Color color = Color(1.0f, 1.0f, 1.0f));
	void load();
	void pushUniform();
	void bind(){ AShader::bind(); } // Hiding Name issue
	void bind(const Attributes & attributes);
	void unbind();

	inline void setAdditiveFactor(float factor) {
		mAdditiveFactor = factor;
	}

	inline void setSpriteId(int sprite){
		mSpriteId = sprite;
	}

private:
	float mAdditiveFactor;
	int mSpriteId;
	Color mColor;
	Attributes mAttributes;
};