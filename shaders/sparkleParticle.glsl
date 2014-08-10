// Sparkle Particle Fragment
///////////////////////////////////////////
uniform sampler2D uTexture; // the texture 
varying vec2 vTexCoord; // Texture coordinates
uniform vec4 uColor;
uniform float uAdditiveFactor; // additive level
uniform int uSpriteId; // sprite id (offset texture coordinate)

// ----------------------------------------
void main() {

  //vec4 col = texture2D( uTexture, gl_TexCoord[0].st);

  float textureOffset = uSpriteId * 0.5;

  vec4 col = texture2D(uTexture, vec2(vTexCoord.x * 0.5 + textureOffset, vTexCoord.y));
  gl_FragColor = col * uColor * uAdditiveFactor;
}

