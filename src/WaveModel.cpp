#include "WaveModel.h"
#include "cinder/app/App.h"

using namespace ci;
using namespace std;

//////////////////////////////////////////////
// Constructor
WaveModel::WaveModel()
{
	mpVertices = nullptr;
	mpNormals = nullptr;
	mpColors = nullptr;
	mpWave = nullptr;
	mpVerticeIndexes = nullptr;
	mFrameCounter = 0;
}

//////////////////////////////////////////////
// Destructor
WaveModel::~WaveModel()
{
	if (mpVertices != nullptr){ delete mpVertices; }
	if (mpNormals != nullptr){ delete mpNormals; }
	if (mpColors != nullptr){ delete mpColors; }
	if (mpWave != nullptr){ delete mpWave; }
	if (mpVerticeIndexes != nullptr){ delete mpVerticeIndexes; }
}

//////////////////////////////////////////////
// Initialize 
void WaveModel::setup(int windowWidth, int windowHeight, int numRows, int numLines, int offsetCameratH){

	mWindow.width = windowWidth;
	mWindow.height = windowHeight;
	mWindow.halfWidth = (int)(mWindow.width * 0.5f);
	mWindow.halfHeight = (int)(mWindow.height * 0.5f);

	mNumRows = numRows;
	mNumLines = numLines;
	mGap = 1.0f * mWindow.width / numLines + 1;
	mNbPoints = mNumRows * mNumLines;
	mNbIndexes = (mNumRows * mNumLines) + (mNumRows - 2) * mNumLines;

	mpVertices = new Vec3f[mNbPoints];
	mpNormals = new Vec3f[mNbPoints];
	mpColors = new ColorA[mNbPoints];
	mpWave = new Vertice[mNbPoints];
	mpVerticeIndexes = new unsigned short[mNbIndexes];

	mOffsetH = offsetCameratH;
	mWaveMotionLimit = (int)(offsetCameratH * 0.9f);

	float	x = 0.0f,
			z = 0.0f,
			y = 0.0f;

	ColorA color(1.0f, 1.0f, 1.0f, 0.1f);

	// Set Wave
	int idVertice = 0;
	for (int i = 0; i < mNumRows; i++){
		for (int j = 0; j < mNumLines; j++){
			idVertice = i * mNumLines + j;
			x = (float)j * mGap - (mNumLines * 0.5f * mGap) + mGap * 0.5f;
			z = (float)i * mGap - mNumRows * mGap;
			y = 0.0f;
			mpWave[idVertice].position = Vec3f(x, y, z);
			mpWave[idVertice].color = color;
			// Keep neighbours
			mpWave[idVertice].neighboursId[0] = (i < (mNumRows - 1)) ? idVertice + mNumLines : -1;
			mpWave[idVertice].neighboursId[1] = (j < (mNumLines - 1)) ? idVertice + 1 : -1;
			mpWave[idVertice].neighboursId[2] = (i > 1) ? idVertice - mNumLines : -1;
			mpWave[idVertice].neighboursId[3] = (j > 1) ? idVertice - 1 : -1;
		}
	}

	// Set Vertices Indexes
	int k = 0;
	mpVerticeIndexes = new unsigned short[(mNumRows - 1) * mNumLines * 2];
	for (int i = 0; i < mNumRows - 1; i++){
		for (int j = 0; j < mNumLines; j++){

			if (i % 2){
				idVertice = i * mNumLines + mNumLines - j - 1;
				mpVerticeIndexes[k++] = idVertice;
				mpVerticeIndexes[k++] = idVertice + mNumLines;
			}
			else{
				idVertice = i * mNumLines + j;
				mpVerticeIndexes[k++] = idVertice;
				mpVerticeIndexes[k++] = idVertice + mNumLines;
			}
		}	
	}
}

//////////////////////////////////////////////
// Compute wave move
void WaveModel::update(float elapsedTime){

	computePositions(elapsedTime);
	computeNormals();
}

//////////////////////////////////////////////
// Draw the wave
void WaveModel::draw(){

	gl::enableAdditiveBlending();
	//gl::enableAlphaBlending();

	// Draw
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, mpVertices);
	glNormalPointer(GL_FLOAT, 0, mpNormals);

	//-----------------------------
#if 0
	gl::enableWireframe();
	glColor3f(0.0f, 1.0f, 0.0f);
	glDrawElements(GL_TRIANGLE_STRIP, mNbIndexes, GL_UNSIGNED_SHORT, mpVerticeIndexes);
	gl::disableWireframe();
#endif
	//-----------------------------

	glDrawElements(GL_TRIANGLE_STRIP, mNbIndexes, GL_UNSIGNED_SHORT, mpVerticeIndexes);

	gl::disableAlphaBlending();
}

//////////////////////////////////////////////
// Compute new elevation of each vertex
void WaveModel::computePositions(float elapsedTime){
	
	float speed = 3.8f;
	elapsedTime = cos(elapsedTime * 0.01f * speed) * 30 + 35;
	mFrameCounter++;

	float mFrameCounterSlow = mFrameCounter * 0.001f * speed;

	float x = 0;
	float border = 0;
	float sinA, sinB;
	float sinWaveX;
	std::ostringstream traceStream;
	
	// Lot of waving parameters... 
	speed += mPerlin.noise(mFrameCounter * 0.0002f, elapsedTime * 0.0002f) * 0.8f;
	float elapsedTimeSlow = elapsedTime * 0.28f;
	float sinTime = sin(elapsedTime);
	float sinTimeSlow = sin(elapsedTimeSlow - elapsedTime);
	float sinTimeAmplitude = sin(elapsedTimeSlow * speed);
	float sinTimeSlowSpeed = sinTimeSlow * speed;
	float horizontaleSpeed = elapsedTime * 200 * speed;
	float zSpeed = elapsedTime * 100 * speed;
	float zNoiseSpeed = elapsedTime * (0.003f * sin(elapsedTimeSlow)) * speed * 0.1f;
	float sinTimeHighAmplitude = sinTime * (8 + 2 * sinTimeAmplitude);
	float frequencyNoise = mPerlin.noise(mFrameCounter * 0.0025f, elapsedTimeSlow);
	float frequency = (0.001f + (0.000028f * sinTimeSlow)) + (elapsedTimeSlow * 0.00008f) + frequencyNoise * 0.00004f;
	float amplitude = mPerlin.noise(sinTimeHighAmplitude * 0.00225f, sinTimeHighAmplitude * 0.0001f);
	amplitude = (160 + (60 * frequencyNoise)) + (elapsedTimeSlow * 54 * zNoiseSpeed) + mPerlin.noise(amplitude * mFrameCounter * 0.00225f, amplitude * elapsedTimeSlow * 0.1f, sinTimeHighAmplitude * 0.00025f) * zSpeed * frequency * 0.12f;

	float moveNoise = mPerlin.noise(elapsedTime, mFrameCounter * 0.0001f) * (sin(elapsedTime * 0.35 + 1) - cos(3 + elapsedTime * 0.25 + sin(elapsedTime) * 0.2f) * 2.3f);

	float waveFirstAmplitude = 0.0f;
	float waveSecondAmplitude = mOffsetH * 0.0035f; 
	float factorX = 0.0005f + (0.0009f * (1 + sinTime));
	float factorX2 = factorX * (0.8f + sinTimeAmplitude);
	float factorZ = 0.01f + (0.0002f * (1 + sinTimeSlow)); 
	float waveOffset = 2 * mWaveMotionLimit * sinTimeAmplitude + factorX2;
	waveOffset += (sinTimeAmplitude * 50);

	// Trace //////////////////////////////////////////////////////////
#ifdef SHOW_WAVE_TRACE
	traceStream << "speed = " << speed << "\n";
	traceStream << "elapsedTime = " << elapsedTime << "\n";
	traceStream << "elapsedTimeSlow = " << elapsedTimeSlow << "\n";
	traceStream << "mFrameCounterSlow = " << mFrameCounterSlow << "\n";
	traceStream << "sinTime = " << sinTime << "\n";
	traceStream << "sinTimeSlow = " << sinTimeSlow << "\n";
	traceStream << "sinTimeAmplitude = " << sinTimeAmplitude << "\n";
	traceStream << "sinTimeSlowSpeed = " << sinTimeSlowSpeed << "\n";
	traceStream << "horizontaleSpeed = " << horizontaleSpeed << "\n";
	traceStream << "zSpeed = " << zSpeed << "\n";
	traceStream << "zNoiseSpeed = " << zNoiseSpeed << "\n";
	traceStream << "sinTimeHighAmplitude = " << sinTimeHighAmplitude << "\n";
	traceStream << "frequencyNoise = " << frequencyNoise << "\n";
	traceStream << "frequency = " << frequency << "\n";
	traceStream << "amplitude = " << amplitude << "\n";
	traceStream << "moveNoise = " << moveNoise << "\n";
	traceStream << "waveSecondAmplitude = " << waveSecondAmplitude << "\n";
	traceStream << "factorX = " << factorX << "\n";
	traceStream << "factorX2 = " << factorX2 << "\n";
	traceStream << "factorZ = " << factorZ << "\n";
	traceStream << "waveOffset = " << waveOffset << "\n";

	mTrace = traceStream.str();
#endif
	// Vertex elevation
	Vec3f position;
	int idVertice = 0;
	float y = 0;

	for (int i = 0; i < mNumRows; i++){
		x = i * 1.0f / mNumRows;
		border = pow(4 * x *(1 - x), 0.125f) * 18;

		for (int j = 0; j < mNumLines; j++){

			idVertice = i * mNumLines + j;
			position = mpWave[idVertice].position;

			sinWaveX = sin((position.x + horizontaleSpeed) * frequency) + moveNoise;
			sinA = sin(-position.z * 0.0001f) * 0.5f;
			sinB = sin(position.x * 0.001f + elapsedTimeSlow);
			waveFirstAmplitude = 35.0f * (sinB + 1.6f + factorZ);
			position.x = position.x * factorX + sinA + sinTimeHighAmplitude;
			position.z = position.z * factorZ * (1.0f + cos(moveNoise) * 0.25f) + elapsedTime;

			// Compute new position
			y = mPerlin.noise(position.x * 0.75f, (position.z * (1.0f + factorZ) + elapsedTime) * 0.78f) * waveFirstAmplitude;
			y += mPerlin.noise(position.x + sinTimeSlow * 0.002f, (position.z * 1.2f + mFrameCounterSlow) * 0.25f) * 60;
			y += sinA * sinB * y * waveSecondAmplitude;
			y += sinWaveX * amplitude + sinTimeSlowSpeed * 1.25f;
			y += waveOffset;
			y += sin(sinTimeSlowSpeed + position.x * 0.08f * position.z * 0.2f) * 60;
			y += border;
			mpWave[idVertice].position.y = y;

			// Set point into buffers
			mpVertices[idVertice] = mpWave[idVertice].position;
		}
	}
}

//////////////////////////////////////////////
// Compute normals of each points
void WaveModel::computeNormals(){
	
	int idVertice = 0;
	int idVertice2 = 0;
	int idVertice3 = 0;
	unsigned char divider = 0;
	Vec3f position;
	Vec3f normal = Vec3f(0., 0., 0.);
	int k = 0;

	for (int i = 0; i < mNumRows; i++){
		for (int j = 0; j < mNumLines; j++){
			
			// Compute normal for 3 first neighbours
			idVertice = i * mNumLines + j;
			position = mpWave[idVertice].position;
			divider = 0;
			
			for (k = 0; k < 4 - 1; k++)
			{
				idVertice2 = mpWave[idVertice].neighboursId[k];
				idVertice3 = mpWave[idVertice].neighboursId[k + 1];
				if (idVertice2 > -1 && idVertice3 > -1)
				{
					mpWave[idVertice].neighboursNormals[k] = (k > 1) 
						? mpWave[idVertice2].neighboursNormals[k] // Use the cached value
						: (mpWave[idVertice2].position - mpWave[idVertice].position) // Compute normal
							.cross(mpWave[idVertice3].position - mpWave[idVertice].position);

					normal += mpWave[idVertice].neighboursNormals[k];
					divider++;
				}
			}

			// Compute normal for the last neighbours
			idVertice2 = mpWave[idVertice].neighboursId[k];
			if (idVertice2 > -1)
			{
				normal += mpWave[idVertice2].neighboursNormals[k]; // Use the cached value
				divider++;
			}

			// Calculate average
			if (divider > 0)
			{
				normal /= divider;
			}
			mpWave[idVertice].normal = normal.normalized();

			// Set Normals stream;
			mpNormals[idVertice] = mpWave[idVertice].normal;
		}
	}
}


//////////////////////////////////////////////
// Write info for debug
void WaveModel::writeInfo(Vec2f position){
	gl::enableAlphaBlending();
	glDisableClientState(GL_COLOR_ARRAY);
	string trace = mTrace;
	trace += "------------------------------------------------\n";//mTrace
	trace += "DEBUG INFORMATIONS: \n";
	gl::drawString(trace, Vec2f(position.x, position.y));
}