// Sparkle Particle Fragment
///////////////////////////////////////////
//uniform sampler2D uTexture; // the texture 
uniform sampler2D uTexture; // the texture 
varying vec2 vTexCoord; // Texture coordinates
varying vec4 vColor;
varying float vAdditiveFactor; // additive level
varying int vSpriteId; // sprite id (offset texture coordinate)

// ----------------------------------------
void main() {

  if(vColor.a){

	  float textureOffset = vSpriteId * 0.5;
	  vec2 texCoord = vec2(gl_PointCoord.x * 0.5 + textureOffset, gl_PointCoord.y);

	  vec4 col = texture2D(uTexture, texCoord) * vColor * vAdditiveFactor;
	  gl_FragColor = col;
  }
}

