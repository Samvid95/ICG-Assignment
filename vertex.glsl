attribute vec4 position;
attribute vec4 normal;
attribute vec2 texCoord;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

<<<<<<< HEAD
varying vec3 varyingNormal;
varying vec3 varyingPosition;

void main() {
	varyingNormal = normalize((normalMatrix * normal).xyz);
	vec4 p = modelViewMatrix * position;
	varyingPosition = p.xyz;
	gl_Position = projectionMatrix * p;
=======
varying vec4 varyingNormal;
varying vec2 varyingTexCoord;

void main() {
	varyingTexCoord = texCoord;
	varyingNormal = normalMatrix * normal;
	gl_Position = projectionMatrix * modelViewMatrix * position;
>>>>>>> 3Dtexture
}