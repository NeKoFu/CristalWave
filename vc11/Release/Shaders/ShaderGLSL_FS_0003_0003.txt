// Simple Fresnel Silhouette approximation
///////////////////////////////////////////
#version 330 core 

in vec3 vN;
in vec3 vV;

uniform float uK; // minnaert roughness  1.5
uniform float uRoughness; // Ward isotropic specular roughness 0.2
uniform float uIoR; // Schlick's fresnel approximation index of refraction 1.5
uniform vec3 uAmbient;
uniform vec3 uDiffuse;
uniform vec3 uSpecular;
uniform vec3 uLightPosition;
uniform float uSpecularAttenuation;
uniform float uTransluscence;

out vec4 out_color;

// ----------------------------------------
// Minnaert limb darkening diffuse term
vec3 minnaert( vec3 L, vec3 Nf, float k) {
  float ndotl = max( 0.0, dot(L, Nf));
  return uDiffuse * pow( ndotl, k);
}

float fresnel( vec3 Nf, vec3 Vf, float ior ) {
  float eta = 1.000293 / ior; // air / eau
  float f = ((1.0 - eta) * (1.0 - eta)) / ((1.0 + eta) * (1.0 + eta));
  float r = f + (1.0 - f) * pow( 1.0 - dot(Vf, Nf), 5.0);
  return r;
}

vec3 specular(vec3 normalDirection, vec3 viewDirection, vec3 lightDirection){

  vec3 specular;
  float ndotl = dot(normalDirection, lightDirection);

  if (ndotl < 0.0) 
     // light source on the wrong side?
  {
     specular = vec3(0.0, 0.0, 0.0); 
        // no specular reflection
  }
  else // light source on the right side
  {
    specular = uSpecularAttenuation * ndotl 
        * uSpecular 
        * pow( max(0.0, dot(reflect(-lightDirection, normalDirection), -viewDirection)), 
        1 / uRoughness);
  }

  return specular;
}

// ----------------------------------------
void main() {
  /**/
  vec3 N = normalize(vN);
  vec3 V = normalize(vV);
  //vec3 L = normalize(vec3(0.0, 10.0, -1.0));
  vec3 L = normalize(uLightPosition);
  
  // Ambiant
  vec3 ambient = uAmbient;

  // Diffuse
  vec3 diffuse = minnaert( L, N, uK); 
  
  // Fresnel 
  float fresnel = fresnel( N, V, uIoR);
  
  // Specular 
  vec3 specular = specular( N, V, L);// * fresnel;
  vec3 color = (ambient + diffuse + specular) + fresnel;
  vec3 alpha = specular * 0.5 + diffuse * 0.5;
  float alphaAttenuation = (N.y - 0.25 + N.z * uSpecularAttenuation * uTransluscence * -N.y * (fresnel + 0.15) * 0.025);
  out_color = vec4( color * alphaAttenuation * alphaAttenuation, (alpha.r + alpha.g + alpha.b) * (0.333 * alphaAttenuation) );

  ///out_color = vec4( color, 1.0);
  /**/
 
 //out_color = vec4( 1.0, 0.0, 0.0, 1.0);
}

