// Sparkle Particle Fragment
///////////////////////////////////////////
#version 330 core

uniform sampler2D uTexture; // the texture 
in vec4 vColor;
in float vSpriteId; // sprite id (offset texture coordinate)
in float vAdditiveFactor; // additive level

out vec4 out_color;

// ----------------------------------------
void main() {

  if(vColor.a > 0.0){

	float textureOffset = 0.5 * vSpriteId;
	vec2 texCoord = vec2(gl_PointCoord.x * 0.5 + textureOffset, gl_PointCoord.y);
	//vec4 type = (vSpriteId > 0.5) ? vec4(0.0, 1.0, 0.0, 0.3) : vec4(1.0, 0.0, 0.0, 0.3);
	////vec4 color = vec4(type.x, type.y, type.z, vColor.a);//  * vAdditiveFactor;
	vec4 color = texture(uTexture, texCoord) * vColor * vAdditiveFactor;
	
	out_color = color;

  }else
  	discard;
}

