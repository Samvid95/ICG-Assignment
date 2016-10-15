attribute vec4 position;
attribute vec4 color;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

varying vec4 varyingColor;

uniform vec4 modelPosition;
void main(){
	varyingColor = color;
	gl_Position = vec4(modelPosition.x, modelPosition.y, modelPosition.z, 0.0) + (projectionMatrix * modelViewMatrix * position);
}