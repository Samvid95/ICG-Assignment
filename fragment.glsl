varying vec3 varyingNormal;
varying vec2 varyingTexCoord;
varying vec3 varyingPosition;

uniform sampler2D diffuseTexture;



uniform vec3 lightDirection;
uniform vec3 specularLightColor;;
uniform vec3 lightColor;

void main() {
	float diffuse = max(0.0, dot(varyingNormal, lightDirection));
	vec3 diffuseColor = lightColor * diffuse;
	
	vec3 v = normalize(-varyingPosition);
	vec3 h = normalize(v + lightDirection);
	
	float specular = pow(max(0.0, dot(h,varyingNormal)),64.0);
	vec3 specularHighlight = specularLightColor * specular;
	
	vec3 intensity = ((texture2D(diffuseTexture, varyingTexCoord).xyz) * diffuseColor) + specularHighlight;
	gl_FragColor = vec4(intensity.xyz,1.0);;
}