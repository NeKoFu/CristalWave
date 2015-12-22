// Simple Fresnel Silhouette approximation
///////////////////////////////////////////
#version 330

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 vV;
out vec3 vN;

void main() {
	vec4 posv4 = vec4(position, 1);
	gl_Position = projectionMatrix * modelViewMatrix * posv4;
	
	vV = (modelViewMatrix * posv4).xyz;
	vN = normal;
}