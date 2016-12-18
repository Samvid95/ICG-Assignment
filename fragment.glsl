varying vec4 varyingNormal;
varying vec2 varyingTexCoord;


uniform sampler2D diffuseTexture;


void main() {
	float diffuse = max(0.0, dot(varyingNormal, vec4(-0.5773, 0.5773, 0.5773, 0.0)));
	gl_FragColor = texture2D(diffuseTexture,varyingTexCoord);
}