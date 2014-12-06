 
varying vec2 vTexCoord;
attribute float particleRadius;
attribute int spriteId; // sprite id (offset texture coordinate)
varying int vSpriteId; // link to fragment
attribute vec4 spriteColor;
varying vec4 vColor;
attribute float additiveFactor;
varying float vAdditiveFactor;


// pass texture coordinates
void main(void)
{
	// fragment parameters
	vSpriteId = spriteId;
	vColor = spriteColor;
	vAdditiveFactor = additiveFactor;

	if(vColor.a){
	vTexCoord = gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_PointSize = particleRadius * 2.0;
	//gl_FrontColor = vec4(1.0,1.0,1.0,1.0);//gl_Color;
	}
	
}
