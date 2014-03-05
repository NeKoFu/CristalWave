
varying vec2 vTexCoord;
 
// remember that you should draw a screen aligned quad
void main(void)
{
   gl_Position = ftransform();
  
   // Clean up inaccuracies
   vec2 Pos;
   Pos = sign(gl_Vertex.xy);

   // Image-space
   vTexCoord = Pos;
}
