varying vec3 varyingNormal;
varying vec2 varyingTexCoord;
varying vec3 varyingPosition;

uniform sampler2D diffuseTexture;

struct Light {
	vec3 lightDirection;
	vec3 lightColor;
	vec3 specularLightColor;
};

uniform Light lights[2];

float attenuate(float dist, float a, float b) {
	return 1.0 / (1.0 + a*dist + b*dist*dist);
}

void main() {
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 specularColor = vec3(0.0, 0.0, 0.0);
	
	
	for(int i=0;i<2;i++){
		vec3 lightPosition = -normalize(varyingPosition-lights[i].lightDirection);
		float diffuse = max(0.0, dot(varyingNormal, lightPosition));
		
		float attenuation = attenuate(distance(varyingPosition, lights[i].lightDirection) / 5.0, 2.7, 5.0);
		diffuseColor += lights[i].lightColor * diffuse * attenuation;
	
		vec3 v = normalize(-varyingPosition);
		vec3 h = normalize(v + lights[i].lightDirection);
		
		float specular = pow(max(0.0, dot(h,varyingNormal)),64.0);
		specularColor += lights[i].specularLightColor * specular;
	}
	
	vec3 intensity = ((texture2D(diffuseTexture, varyingTexCoord).xyz) * diffuseColor) + specularColor;
	gl_FragColor = vec4(intensity.xyz,1.0);;
}