
#include "SparkleShader.h"

SparkleShader::SparkleShader(float additiveFactor /* = 1.0f*/, Color color /* Color(1.0f, 1.0f, 1.0f) */){
	mShaderName = "Sparkle";
	mIdRessourceVertexShader = RES_VERT_GLSL_SPARKLE_SPRITE;
	mIdRessourceFragmentShader = RES_FRAG_GLSL_SPARKLE_SPRITE;

	setAdditiveFactor(additiveFactor);
	mColorAmbient = color;
}

void SparkleShader::load() {
	AShader::load();
};

void SparkleShader::pushUniform()
{
	mShaderProgram.uniform("uAdditiveFactor", mAdditiveFactor);
	mShaderProgram.uniform("uSpriteId", mSpriteId);
	mShaderProgram.uniform("uColor", Vec4f(mColorAmbient.r, mColorAmbient.g, mColorAmbient.b, mOpacity));
}

void SparkleShader::bind(const Attributes & attributes)
{
	AShader::bind();
	mAttributes = attributes;

	mAttributes.texture.enableAndBind();

	GLint particuleRadiusInShader = mShaderProgram.getAttribLocation("particleRadius");
	GLint spriteIdInShader = mShaderProgram.getAttribLocation("spriteId");
	GLint spriteColorInShader = mShaderProgram.getAttribLocation("spriteColor");
	GLint additiveFactorInShader = mShaderProgram.getAttribLocation("additiveFactor");
	
	glEnableVertexAttribArray(particuleRadiusInShader);
	glVertexAttribPointer(particuleRadiusInShader, 1, GL_FLOAT, false, 0, mAttributes.radius);

	glEnableVertexAttribArray(spriteIdInShader);
	glVertexAttribPointer(spriteIdInShader, 1, GL_INT, false, 0, mAttributes.spriteIDs);

	glEnableVertexAttribArray(spriteColorInShader);
	glVertexAttribPointer(spriteColorInShader, 4, GL_FLOAT, false, 0, mAttributes.colors);

	glEnableVertexAttribArray(additiveFactorInShader);
	glVertexAttribPointer(additiveFactorInShader, 1, GL_FLOAT, false, 0, mAttributes.additiveFactors);

	glEnable(GL_POINT_SPRITE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mAttributes.positions);

	// TIME for DEBUG
	//float elapsedTime = (float)ci::app::getElapsedSeconds() * 0.1f;
	//mShaderProgram.uniform("uTime", elapsedTime);

	gl::enableAdditiveBlending();
}

void SparkleShader::unbind()
{
	gl::enableAlphaBlending(false);
	glDisable(GL_POINT_SPRITE);

	if (mAttributes.texture){
		mAttributes.texture.disable();
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	GLint particuleRadiusInShader = mShaderProgram.getAttribLocation("particleRadius");
	glDisableVertexAttribArray(particuleRadiusInShader);
	AShader::unbind();
}
