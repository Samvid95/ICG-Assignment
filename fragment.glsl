varying vec3 varyingNormal;
varying vec3 varyingPosition;
uniform vec3 uColor;


struct Light{
	vec3 lightPosition;
	vec3 lightColor;
	vec3 specularLightColor;
};

uniform Light lights[2];

void main() {
	
	vec3 diffuseColor = vec3(0.0, 0.0, 0.0);
	vec3 specularColor = vec3(0.0, 0.0, 0.0);
	for(int i=0; i< 2; i++) {
		vec3 lightDirection = - normalize(varyingPosition - lights[i].lightPosition);
		float diffuse = max(0.0, dot(varyingNormal, lightDirection));
		diffuseColor += lights[i].lightColor * diffuse;
		vec3 v = normalize(-varyingPosition);
		vec3 h = normalize(v + lightDirection);
		float specular = pow(max(0.0, dot(h, varyingNormal)), 2.0);
		specularColor += lights[i].specularLightColor * specular;
	}
	vec3 intensity = (uColor * diffuseColor) + specularColor;
	gl_FragColor = vec4(intensity.xyz, 1.0);
}