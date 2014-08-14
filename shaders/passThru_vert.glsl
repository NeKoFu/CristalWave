 
varying vec2 vTexCoord;

// pass texture coordinates
void main(void)
{
	vTexCoord = gl_MultiTexCoord0;
	gl_Position = ftransform();
}
