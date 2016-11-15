varying vec4 varyingNormal;
varying vec2 varyingTexCoord;

uniform sampler2D diffuseTexture;
uniform vec3 uColor;

void main() {
	float diffuse = max(0.0, dot(varyingNormal, vec4(-0.5773, 0.5773, 0.5773, 0.0)));
	vec3 intensity = uColor * diffuse;
	//gl_FragColor = vec4(varyingTexCoord.x,0.0,varyingTexCoord.y, 1.0);
	gl_FragColor = texture2D(diffuseTexture,varyingTexCoord);
}