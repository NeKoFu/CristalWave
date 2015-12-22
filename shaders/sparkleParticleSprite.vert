// Sparkle Particle Vertex
///////////////////////////////////////////
#version 330

uniform mat4 projectionModelViewMatrix;

// attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 spriteColor;
layout (location = 2) in int spriteId; // sprite id (offset texture coordinate)
layout (location = 3) in float particleRadius;
layout (location = 4) in float additiveFactor;

out vec4 vColor;
out float vSpriteId; // link to fragment
out float vAdditiveFactor;



// pass texture coordinates
void main(void)
{
    // fragment parameters
    vSpriteId = float(spriteId);
    vColor = spriteColor;
    vAdditiveFactor = additiveFactor;

    if(vColor.a > 0.0){

        gl_Position = projectionModelViewMatrix * vec4(position, 1.0);
        gl_PointSize = particleRadius;// * 20.0;
        //gl_PointSize = 80.0 * (1.0 + vSpriteId);
    }
    
}
