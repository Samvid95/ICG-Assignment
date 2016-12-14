attribute vec4 position;
attribute vec4 normal;
attribute vec2 texCoord;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

varying vec4 varyingNormal;
varying vec2 varyingTexCoord;
varying vec4 projectedTexCoord;

void main() {
	varyingTexCoord = texCoord;
	varyingNormal = normalMatrix * normal;
	projectedTexCoord = position;
	gl_Position = projectionMatrix * modelViewMatrix * position;
}