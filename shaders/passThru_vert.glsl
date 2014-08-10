 
varying vec2 vTexCoord;

// pass texture coordinates
void main(void)
{
	//gl_TexCoord[0] = gl_MultiTexCoord0;
	vTexCoord = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
